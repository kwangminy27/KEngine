#include "Engine.h"
#include "collision_manager.h"

#include "Object/Actor/actor.h"
#include "Object/Component/Collider/collider.h"

std::unique_ptr<K::CollisionGroup> K::CollisionManager::collision_group_dummy_{};

void K::CollisionManager::Initialize()
{
	try
	{
		Vector3 resolution = { static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT), 1.f };

		_CreateCollisionGroup(UI, Vector3::Zero, resolution, 4, 4, 1);
		_CreateCollisionGroup(DEFAULT, Vector3::Zero, resolution, 4, 4, 1);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "CollisionManager::Initialize" << std::endl;
	}
}

void K::CollisionManager::Collision(float _time)
{
	for (auto iter = collision_group_map_.begin(); iter != collision_group_map_.end(); ++iter)
	{
		auto const& collision_group = iter->second;

		for (auto i = 0; i < collision_group->total_count; ++i)
		{
			auto& section = collision_group->section[i];

			for (auto const& collider : section)
				collider->_UpdateCollidedCollider(_time);

			for (auto j = 0; j < section.size() - 1; ++j)
			{
				for (auto k = j + 1; k < section.size(); ++k)
				{
					auto const& src = section.at(j);
					auto const& dest = section.at(k);

					if (TAG_STATE::DISABLED == src->tag_state() || TAG_STATE::DEAD == src->tag_state())
						continue;

					if (TAG_STATE::DISABLED == dest->tag_state() || TAG_STATE::DEAD == dest->tag_state())
						continue;

					if (src->_Collision(dest, _time))
					{
						if (src->_IsCollidedCollider(dest))
						{
							dest->_OnCollisionStay(src, _time);
							src->_OnCollisionStay(dest, _time);
						}
						else
						{
							dest->_OnCollisionEnter(src, _time);
							src->_OnCollisionEnter(dest, _time);

							dest->_AddCollidedCollider(src);
							src->_AddCollidedCollider(dest);
						}
					}
					else
					{
						if (src->_IsCollidedCollider(dest))
						{
							dest->_OnCollisionLeave(src, _time);
							src->_OnCollisionLeave(dest, _time);

							dest->_RemoveCollidedCollider(src);
							src->_RemoveCollidedCollider(dest);
						}
					}
				}
			}

			section.clear();
		}
	}
}

std::unique_ptr<K::CollisionGroup> const& K::CollisionManager::FindCollisionGroup(std::string const& _tag) const
{
	auto iter = collision_group_map_.find(_tag);

	if (iter == collision_group_map_.end())
		return collision_group_dummy_;

	return iter->second;
}

void K::CollisionManager::AddCollider(APTR const& _actor)
{
	auto const& collider_list = _actor->FindComponent(COLLIDER);

	if (collider_list.empty())
		return;

	for (auto iter = collider_list.begin(); iter != collider_list.end(); ++iter)
	{
		auto const& collider = static_cast<Collider*>(*iter);
		auto const& collision_group = FindCollisionGroup(collider->group_tag_);

		if (nullptr == collision_group)
			return;

		Vector3 min = collider->min_;
		Vector3 max = collider->max_;

		auto min_x_idx = static_cast<int>(min.x - collision_group->min.x) / static_cast<int>(collision_group->section_diagonal_length.x);
		auto min_y_idx = static_cast<int>(min.y - collision_group->min.y) / static_cast<int>(collision_group->section_diagonal_length.y);
		auto min_z_idx = static_cast<int>(min.z - collision_group->min.z) / static_cast<int>(collision_group->section_diagonal_length.z);

		auto max_x_idx = static_cast<int>(max.x - collision_group->min.x) / static_cast<int>(collision_group->section_diagonal_length.x);
		auto max_y_idx = static_cast<int>(max.y - collision_group->min.y) / static_cast<int>(collision_group->section_diagonal_length.y);
		auto max_z_idx = static_cast<int>(max.y - collision_group->min.y) / static_cast<int>(collision_group->section_diagonal_length.z);

		collider->section_idx_list_.clear();

		for (auto z = min_z_idx; z <= max_z_idx; ++z)
		{
			if (z < 0 || z >= collision_group->z_count)
				continue;

			for (auto y = min_y_idx; y <= max_y_idx; ++y)
			{
				if (y < 0 || y >= collision_group->y_count)
					continue;

				for (auto x = min_x_idx; x <= max_x_idx; ++x)
				{
					if (x < 0 || x >= collision_group->x_count)
						continue;

					auto idx = (collision_group->x_count * collision_group->y_count * z) + (collision_group->x_count * y) + x;

					collider->_AddSectionIdx(idx);
					collision_group->section[idx].push_back(collider);
				}
			}
		}
	}
}

void K::CollisionManager::_Finalize()
{
}

void K::CollisionManager::_CreateCollisionGroup(std::string const& _tag, Vector3 const& _min, Vector3 const& _max, int _x_count, int _y_count, int _z_count)
{
	if (FindCollisionGroup(_tag))
		throw std::exception{ "CollisionManager::_CreateCollisionGroup" };

	auto collision_group = std::make_unique<CollisionGroup>();

	collision_group->min = _min;
	collision_group->max = _max;
	collision_group->x_count = _x_count;
	collision_group->y_count = _y_count;
	collision_group->z_count = _z_count;
	collision_group->total_count = _x_count * _y_count * _z_count;
	collision_group->section_diagonal_length = (_max - _min) / Vector3{ static_cast<float>(_x_count), static_cast<float>(_y_count), static_cast<float>(_z_count) };

	collision_group->section = std::unique_ptr<CollisionSection[], std::function<void(CollisionSection*)>>{ new CollisionSection[collision_group->total_count], [](CollisionSection* _p) {
		delete[] _p;
	} };

	for (auto i = 0; i < collision_group->total_count; ++i)
		collision_group->section[i].reserve(100);

	collision_group_map_.insert(std::make_pair(_tag, std::move(collision_group)));
}
