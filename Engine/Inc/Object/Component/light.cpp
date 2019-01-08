#include "Engine.h"
#include "light.h"

void K::Light::Initialize()
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
		std::cout << "Light::Initialize" << std::endl;
	}
}

void K::Light::Input(float _time)
{
}

void K::Light::Update(float _time)
{
}

void K::Light::Collision(float _time)
{
}

void K::Light::Render(float _time)
{
}

K::CPTR K::Light::Clone() const
{
	return CPTR{ new Light, [](Component* _p) {
		static_cast<Light*>(_p)->_Finalize();
		delete static_cast<Light*>(_p);
	} };
}

void K::Light::Serialize(InputMemoryStream& _imstream)
{
}

void K::Light::Serialize(OutputMemoryStream& _omstream)
{
}

K::Light::Light(Light const& _other) : Component(_other)
{
}

K::Light::Light(Light&& _other) noexcept : Component(std::move(_other))
{
}

void K::Light::_Finalize()
{
}
