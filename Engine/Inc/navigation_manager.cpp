#include "Engine.h"
#include "navigation_manager.h"

#include "Object/Actor/tile_map_actor.h"

void K::NavigationManager::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "NavigationManager::Initialize" << std::endl;
	}
}

bool K::NavigationManager::Route(Vector3 const& _start, Vector3 const& _end)
{
	std::shared_ptr<TileMapActor> tile_map{};

	for (auto iter = tile_map_list_.begin(); iter != tile_map_list_.end(); ++iter)
	{
		if ((*iter)->InBounds(_start) && (*iter)->InBounds(_end))
		{
			tile_map = *iter;
			break;
		}
	}

	if (nullptr == tile_map)
		return false;

	static auto IsIn = [](std::pair<int, int> const& _src_idx, std::list<std::shared_ptr<A_STAR_DESC>> const& _dest_list) {
		for (auto const& element : _dest_list)
		{
			if (element->idx == _src_idx)
				return true;
		}

		return false;
	};

	std::shared_ptr<A_STAR_DESC> current{};
	std::list<std::shared_ptr<A_STAR_DESC>> open_list{};
	std::list<std::shared_ptr<A_STAR_DESC>> close_list{};

	auto const& start_idx = tile_map->GetTileIndex(_start);
	auto const& end_idx = tile_map->GetTileIndex(_end);

	if (start_idx == end_idx)
		return false;

	switch (tile_map->GetTileOption(end_idx))
	{
	case TILE_OPTION::BLOCKED:
	case TILE_OPTION::MAX:
		return false;
	}

	current = std::make_shared<A_STAR_DESC>();
	current->idx = start_idx;

	open_list.push_back(current);

	while (current->idx != end_idx)
	{
		auto const& adjacency_list = tile_map->GetTileAdjacencyList(current->idx);

		for (auto const& idx : adjacency_list)
		{
			if (IsIn(idx, close_list))
				continue;

			if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(idx))
				continue;

			// 대각 타일 처리
			switch (idx.second - current->idx.second)
			{
			case -1:
				switch (idx.first - current->idx.first)
				{
				case -1:
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first - 1, current->idx.second }))
						continue;
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first, current->idx.second - 1 }))
						continue;
					break;

				case 1:
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first + 1, current->idx.second }))
						continue;
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first, current->idx.second - 1 }))
						continue;
					break;
				}
				break;

			case 1:
				switch (idx.first - current->idx.first)
				{
				case -1:
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first - 1, current->idx.second }))
						continue;
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first, current->idx.second + 1 }))
						continue;
					break;

				case 1:
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first + 1, current->idx.second }))
						continue;
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption(std::pair{ current->idx.first, current->idx.second + 1 }))
						continue;
					break;
				}
				break;
			}

			auto current_position = tile_map->GetTilePosition(current->idx);
			auto adjacency_position = tile_map->GetTilePosition(idx);
			auto G = current->G + Vector3::Distance(current_position, adjacency_position);

			if (IsIn(idx, open_list))
			{
				auto iter = std::find_if(open_list.begin(), open_list.end(), [&idx](std::shared_ptr<A_STAR_DESC> const& _p) {
					return _p->idx == idx;
				});

				auto adjacency = *iter;

				if (G < adjacency->G)
				{
					adjacency->G = G;
					adjacency->F = G + adjacency->H;
					adjacency->parent = current;
				}

				continue;
			}

			std::shared_ptr<A_STAR_DESC> a_star_desc{};
			a_star_desc = std::make_shared<A_STAR_DESC>();
			a_star_desc->G = G;
			a_star_desc->H = Vector3::Distance(adjacency_position, _end);
			a_star_desc->F = a_star_desc->G + a_star_desc->H;
			a_star_desc->idx = idx;
			a_star_desc->parent = current;

			open_list.push_back(std::move(a_star_desc));
		}

		if (open_list.empty())
			return false;

		open_list.sort([](std::shared_ptr<A_STAR_DESC> const& _p1, std::shared_ptr<A_STAR_DESC> const& _p2) {
			return _p1->F < _p2->F;
		});

		current = open_list.front();
		open_list.pop_front();

		close_list.push_back(current);
	}

	move_path_list_.clear();

	std::pair<int, int> pred{};
	std::pair<int, int> curr{};
	std::pair<int, int> succ{};
	std::stack<std::pair<int, int>> route{};
	std::list<std::pair<int, int>> optimized_route{};

	while (current->idx != start_idx)
	{
		route.push(current->idx);
		current = current->parent;
	}

	if (route.size() == 1)
		optimized_route.push_back(route.top());
	else
	{
		pred = start_idx;

		while (route.size() > 1)
		{
			curr = route.top();
			route.pop();
			succ = route.top();

			auto x_minmax = std::minmax(pred.first, succ.first);
			auto y_minmax = std::minmax(pred.second, succ.second);

			bool block_flag{};

			for (auto i = y_minmax.first; i <= y_minmax.second; ++i)
			{
				for (auto j = x_minmax.first; j <= x_minmax.second; ++j)
				{
					if (TILE_OPTION::BLOCKED == tile_map->GetTileOption({ j, i }))
					{
						block_flag = true;
						break;
					}
				}

				if (block_flag)
					break;
			}

			if (block_flag)
			{
				optimized_route.push_back(curr);

				pred = curr;
			}
		}

		optimized_route.push_back(route.top());
	}

	for (auto const& e : optimized_route)
		move_path_list_.push_back(tile_map->GetTilePosition(e));

	move_path_list_.pop_back();
	move_path_list_.push_back(_end);

	return true;
}

void K::NavigationManager::AddTileMap(std::shared_ptr<TileMapActor> const& _tile_map)
{
	tile_map_list_.push_back(_tile_map);
}

std::list<K::Vector3> const& K::NavigationManager::move_path_list() const
{
	return move_path_list_;
}

void K::NavigationManager::_Finalize()
{
}
