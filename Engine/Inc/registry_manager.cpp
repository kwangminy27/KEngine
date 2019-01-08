#include "Engine.h"
#include "registry_manager.h"

std::function<K::APTR(K::TAG const&)> K::RegistryManager::actor_generator_dummy_{};

void K::RegistryManager::Initialize()
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
		std::cout << "RegistryManager::Initialize" << std::endl;
	}
}

std::function<K::APTR(K::TAG const&)> const& K::RegistryManager::FindActorGenerator(std::string const& _tag) const
{
	auto iter = actor_generator_map_.find(_tag);

	if (iter == actor_generator_map_.end())
		return actor_generator_dummy_;

	return iter->second;
}

void K::RegistryManager::AddActorGenerator(std::string const& _tag, std::function<APTR(TAG const&)> const& _function)
{
	if (FindActorGenerator(_tag))
		throw std::exception{ "RegistryManager::_CreateActorGenerator" };

	actor_generator_map_.insert(std::make_pair(_tag, _function));
}

void K::RegistryManager::_Finalize()
{
}
