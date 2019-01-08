#include "Engine.h"
#include "level.h"

#include "layer.h"

std::shared_ptr<K::Layer> K::Level::layer_dummy_{};

void K::Level::Initialize()
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
		std::cout << "Level::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Layer> const& K::Level::FindLayer(TAG const& _tag) const
{
	auto iter = std::find_if(layer_list_.begin(), layer_list_.end(), [&_tag](std::shared_ptr<Layer> const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == layer_list_.end())
		return layer_dummy_;

	return *iter;
}

K::APTR const& K::Level::FindActor(TAG const& _tag) const
{
	for (auto const& layer : layer_list_)
	{
		if (auto const& actor = layer->FindActor(_tag))
			return actor;
	}

	return Layer::actor_dummy_;
}

std::shared_ptr<K::Layer> K::Level::CreateLayer(TAG const& _tag)
{
	auto layer = std::shared_ptr<Layer>{ new Layer, [](Layer* _p) {
		_p->_Finalize();
		delete _p;
	} };

	layer->set_tag(_tag);
	layer->set_level(shared_from_this());

	layer->Initialize();

	layer_list_.push_back(layer);

	layer_list_.sort([](std::shared_ptr<Layer> const& _p1, std::shared_ptr<Layer> const& _p2) {
		return _p1->tag().second < _p2->tag().second;
	});

	return layer;
}

K::Level::Level(Level&& _other) noexcept : Tag(std::move(_other))
{
	layer_list_ = std::move(_other.layer_list_);
}

void K::Level::_Finalize()
{
}

void K::Level::_Input(float _time)
{
}

void K::Level::_Update(float _time)
{
}

void K::Level::_Collision(float _time)
{
}

void K::Level::_Render(float _time)
{
}

void K::Level::__Input(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Input(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::__Update(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Update(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::__Collision(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Collision(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::__Render(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Render(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}
