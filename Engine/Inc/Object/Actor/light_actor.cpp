#include "Engine.h"
#include "light_actor.h"

#include "Object/object_manager.h"
#include "Object/Component/light.h"

void K::LightActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto light = object_manager->CreateComponent<Light>(TAG{ LIGHT, 0 });
		AddComponent(light);

		set_render_group_type(RENDER_GROUP_TYPE::LIGHT);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "LightActor::Initialize" << std::endl;
	}
}

K::APTR K::LightActor::Clone() const
{
	return APTR{ new LightActor{ *this }, [](LightActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::LightActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::LightActor::Serialize(OutputMemoryStream& _omstream)
{
}

K::LightActor::LightActor(LightActor const& _other) : Actor(_other)
{
}

K::LightActor::LightActor(LightActor&& _other) noexcept : Actor(std::move(_other))
{
}

void K::LightActor::_Finalize()
{
}
