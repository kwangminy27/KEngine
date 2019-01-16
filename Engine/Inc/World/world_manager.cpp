#include "Engine.h"
#include "world_manager.h"

#include "level.h"
#include "layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"

std::shared_ptr<K::Level> K::WorldManager::level_dummy_{};
std::shared_ptr<K::CameraActor> K::WorldManager::camera_dummy_{};

void K::WorldManager::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto ui_camera = std::static_pointer_cast<CameraActor>(object_manager->CreateActor<CameraActor>({ UI_CAMERA, 0 }));
		ui_camera->CreateProjection(static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT), 0.f, 1000.f);
		camera_list_.push_back(ui_camera);

		auto default_camera = std::static_pointer_cast<CameraActor>(object_manager->CreateActor<CameraActor>({ DEFAULT_CAMERA, 0 }));
		camera_list_.push_back(default_camera);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "WorldManager::Initialize" << std::endl;
	}
}

void K::WorldManager::Input(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		(*iter)->_Input(_time);

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
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Update(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		(*iter)->_Update(_time);

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
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Collision(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		(*iter)->_Collision(_time);

		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->__Collision(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

void K::WorldManager::Render(float _time)
{
	for (auto iter = level_list_.begin(); iter != level_list_.end();)
	{
		(*iter)->_Render(_time);

		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->__Render(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = level_list_.erase(iter);
			break;
		}
	}
}

std::shared_ptr<K::Level> const& K::WorldManager::FindLevel(TAG const& _tag) const
{
	auto iter = std::find_if(level_list_.begin(), level_list_.end(), [&_tag](std::shared_ptr<Level> const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == level_list_.end())
		return level_dummy_;

	return *iter;
}

std::shared_ptr<K::Layer> const& K::WorldManager::FindLayer(TAG const& _tag) const
{
	for (auto const& level : level_list_)
	{
		if (auto const& layer = level->FindLayer(_tag))
			return layer;
	}

	return Level::layer_dummy_;
}

K::APTR const& K::WorldManager::FindActor(TAG const& _tag) const
{
	for (auto const& level : level_list_)
	{
		if (auto const& actor = level->FindActor(_tag))
			return actor;
	}

	return Layer::actor_dummy_;
}

std::shared_ptr<K::CameraActor> const& K::WorldManager::FindCamera(TAG const& _tag) const
{
	auto iter = std::find_if(camera_list_.begin(), camera_list_.end(), [&_tag](std::shared_ptr<CameraActor> const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == camera_list_.end())
		return camera_dummy_;
	
	return *iter;
}

void K::WorldManager::_Finalize()
{
}

void K::WorldManager::_AddCamera(std::shared_ptr<CameraActor> const& _camera)
{
	camera_list_.push_back(_camera);
}
