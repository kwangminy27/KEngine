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

		auto global_ambient_light = object_manager->CreateActor<LightActor>(TAG{ GLOBAL_AMBIENT_LIGHT, 0 });
		auto const& global_ambient_light_component = CPTR_CAST<Light>(global_ambient_light->FindComponent(TAG{ LIGHT, 0 }));
		global_ambient_light_component->set_type(static_cast<int>(LIGHT_TYPE::DIRECTIONAL));
		global_ambient_light_component->set_direction(Vector3{ 1.f, -1.f, -1.f });
		global_ambient_light_component->set_ambient(Vector4{ 0.2f, 0.2f, 0.2f, 1.f });
		global_ambient_light_component->set_diffuse(DirectX::Colors::Black.v);
		global_ambient_light_component->set_specular(DirectX::Colors::Black.v);
		default_layer->AddActor(global_ambient_light);

		auto light = object_manager->CreateActor<LightActor>(TAG{ DEFAULT_LIGHT, 0 });
		default_layer->AddActor(light);
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
