#include "Engine.h"
#include "replication_manager.h"

#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/Actor/actor.h"
#include "registry_manager.h"

void K::ReplicationManager::Initialize()
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
		std::cout << "ReplicationManager::Initialize" << std::endl;
	}
}

void K::ReplicationManager::SendWorld(OutputMemoryStream& _omstream)
{
	auto size = replicable_actor_list_.size();
	_omstream.Serialize(size);

	for (auto& actor : replicable_actor_list_)
	{
		auto tag = actor->tag();
		_omstream.Serialize(tag.first);
		_omstream.Serialize(tag.second);

		actor->Serialize(_omstream);
	}
}

void K::ReplicationManager::ReceiveWorld(InputMemoryStream& _imstream)
{
	auto const& world_manager = WorldManager::singleton();
	auto const& registry_manager = RegistryManager::singleton();

	auto const& layer = world_manager->FindLayer({ "DefaultLayer", 0 });

	size_t size{};
	_imstream.Serialize(size);

	for (auto i = 0; i < size; ++i)
	{
		TAG tag{};
		_imstream.Serialize(tag.first);
		_imstream.Serialize(tag.second);

		auto actor = registry_manager->FindActorGenerator(tag.first)(tag);
		actor->Serialize(_imstream);

		layer->AddActor(actor);
	}
}

void K::ReplicationManager::AddActor(APTR const& _actor)
{
	replicable_actor_list_.push_back(_actor);
}

void K::ReplicationManager::RemoveActor(APTR const& _actor)
{
	replicable_actor_list_.remove(_actor);
}

void K::ReplicationManager::_Finalize()
{
}
