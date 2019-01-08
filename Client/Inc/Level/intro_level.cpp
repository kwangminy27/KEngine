#include "Client.h"
#include "intro_level.h"

#include "Actor/player.h"

void K::IntroLevel::Initialize()
{
	try
	{
		auto background_layer = CreateLayer(TAG{ BACKGROUND, 10 });
		auto default_layer = CreateLayer(TAG{ DEFAULT, 15 });
		auto ui_layer = CreateLayer(TAG{ UI, 20 });

		auto const& object_manager = ObjectManager::singleton();

		auto player = object_manager->CreateActor<Player>(TAG{ "Player", 0 });
		default_layer->AddActor(player);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "IntroLevel::Initialize" << std::endl;
	}
}

K::IntroLevel::IntroLevel(IntroLevel&& _other) noexcept : Level(std::move(_other))
{
}

void K::IntroLevel::_Finalize()
{
}

void K::IntroLevel::_Input(float _time)
{
}

void K::IntroLevel::_Update(float _time)
{
}

void K::IntroLevel::_Collision(float _time)
{
}

void K::IntroLevel::_Render(float _time)
{
}
