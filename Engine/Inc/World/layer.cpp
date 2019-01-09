#include "Engine.h"
#include "layer.h"

#include "world_manager.h"
#include "level.h"
#include "Object/Actor/actor.h"
#include "Rendering/rendering_manager.h"
#include "collision_manager.h"
#include "replication_manager.h"

K::APTR K::Layer::actor_dummy_{};

void K::Layer::Initialize()
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
		std::cout << "Layer::Initialize" << std::endl;
	}
}

K::APTR const& K::Layer::FindActor(TAG const& _tag) const
{
	auto iter = std::find_if(actor_list_.begin(), actor_list_.end(), [&_tag](APTR const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == actor_list_.end())
		return actor_dummy_;

	return *iter;
}

void K::Layer::AddActor(APTR const& _actor)
{
	_actor->set_level(level());
	_actor->set_layer(shared_from_this());

	actor_list_.push_back(_actor);

	for (auto const& child : _actor->child_list())
		AddActor(child);
}

void K::Layer::RemoveActor(APTR const& _actor)
{
	_actor->set_level(WorldManager::level_dummy_);
	_actor->set_layer(Level::layer_dummy_);

	for (auto const& child : _actor->child_list())
		RemoveActor(child);

	actor_list_.remove(_actor);
}

std::shared_ptr<K::Level> K::Layer::level() const
{
	return level_.lock();
}

void K::Layer::set_level(std::shared_ptr<Level> const& _level)
{
	level_ = _level;
}

K::Layer::Layer(Layer&& _other) noexcept : Tag(std::move(_other))
{
	level_ = std::move(_other.level_);
	actor_list_ = std::move(_other.actor_list_);
}

void K::Layer::_Finalize()
{
}

void K::Layer::_Input(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->__Input(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			ReplicationManager::singleton()->RemoveActor(*iter);

			if (auto parent = (*iter)->parent())
				parent->RemoveChild(*iter);

			for (auto const& child : (*iter)->child_list())
				child->set_parent(actor_dummy_);

			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Update(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->__Update(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			ReplicationManager::singleton()->RemoveActor(*iter);

			if (auto parent = (*iter)->parent())
				parent->RemoveChild(*iter);

			for (auto const& child : (*iter)->child_list())
				child->set_parent(actor_dummy_);

			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Collision(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			//(*iter)->__Collision(_time);
			CollisionManager::singleton()->AddCollider(*iter);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			ReplicationManager::singleton()->RemoveActor(*iter);

			if (auto parent = (*iter)->parent())
				parent->RemoveChild(*iter);

			for (auto const& child : (*iter)->child_list())
				child->set_parent(actor_dummy_);

			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Render(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			//(*iter)->__Render(_time);
			RenderingManager::singleton()->AddActor(*iter);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			ReplicationManager::singleton()->RemoveActor(*iter);

			if (auto parent = (*iter)->parent())
				parent->RemoveChild(*iter);

			for (auto const& child : (*iter)->child_list())
				child->set_parent(actor_dummy_);

			iter = actor_list_.erase(iter);
			break;
		}
	}
}
