#include "Engine.h"
#include "object_manager.h"

void K::ObjectManager::Initialize()
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
		std::cout << "ObjectManager::Initialize" << std::endl;
	}
}

void K::ObjectManager::_Finalize()
{
}
