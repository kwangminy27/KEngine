#include "Engine.h"
#include "light.h"

#include "Rendering/rendering_manager.h"

void K::Light::Initialize()
{
	try
	{
		using namespace DirectX::Colors;

		//set_type(static_cast<int>(LIGHT_TYPE::DIRECTIONAL));
		//set_ambient(Vector4{ 0.2f, 0.2f, 0.2f, 1.f });
		//set_diffuse(White.v);
		//set_specular(White.v);
		//set_position(Vector3::Zero);
		//set_direction(Vector3::UnitZ);
		//set_range(0.f);
		//set_falloff(0.f);
		//set_in_angle(0.f);
		//set_out_angle(0.f);
		//set_attenuation(Vector3::Zero);

		//set_type(static_cast<int>(LIGHT_TYPE::POINT));
		//set_ambient(Vector4{ 0.2f, 0.2f, 0.2f, 1.f });
		//set_diffuse(Pink.v);
		//set_specular(Pink.v);
		//set_position(Vector3{ 2.f, 2.f, -1.f });
		//set_direction(Vector3::Zero);
		//set_range(10.f);
		//set_falloff(0.f);
		//set_in_angle(0.f);
		//set_out_angle(0.f);
		//set_attenuation(Vector3{ 0.1f, 0.1f, 0.1f });

		set_type(static_cast<int>(LIGHT_TYPE::SPOT));
		set_ambient(Vector4{ 0.2f, 0.2f, 0.2f, 1.f });
		set_diffuse(White.v);
		set_specular(White.v);
		set_position(-Vector3::UnitZ);
		set_direction(Vector3::UnitZ);
		set_attenuation(Vector3::One);
		set_falloff(10.f);
		set_range(100.f);
		set_in_angle(0.f);
		set_out_angle(0.f);
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

void K::Light::UpdateConstantBuffer()
{
	LightConstantBuffer light_CB{};

	light_CB.ambient = ambient_;
	light_CB.diffuse = diffuse_;
	light_CB.specular = specular_;
	light_CB.position = position_;
	light_CB.direction = direction_;
	light_CB.attenuation = attenuation_;
	light_CB.falloff = falloff_;
	light_CB.type = type_;
	light_CB.range = range_;
	light_CB.in_angle = in_angle_;
	light_CB.out_angle = out_angle_;

	RenderingManager::singleton()->UpdateConstantBuffer(LIGHT, &light_CB);
}

void K::Light::set_type(int _type)
{
	type_ = _type;
}

void K::Light::set_ambient(Vector4 const& _ambient)
{
	ambient_ = _ambient;
}

void K::Light::set_diffuse(Vector4 const& _diffuse)
{
	diffuse_ = _diffuse;
}

void K::Light::set_specular(Vector4 const& _specular)
{
	specular_ = _specular;
}

void K::Light::set_position(Vector3 const& _position)
{
	position_ = _position;
}

void K::Light::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

void K::Light::set_range(float _range)
{
	range_ = _range;
}

void K::Light::set_falloff(float _falloff)
{
	falloff_ = _falloff;
}

void K::Light::set_in_angle(float _in_angle)
{
	in_angle_ = _in_angle;
}

void K::Light::set_out_angle(float _out_angle)
{
	out_angle_ = _out_angle;
}

void K::Light::set_attenuation(Vector3 const& _attenuation)
{
	attenuation_ = _attenuation;
}

K::Light::Light(Light const& _other) : Component(_other)
{
	type_ = _other.type_;
	ambient_ = _other.ambient_;
	diffuse_ = _other.diffuse_;
	specular_ = _other.specular_;
	position_ = _other.position_;
	direction_ = _other.direction_;
	range_ = _other.range_;
	falloff_ = _other.falloff_;
	in_angle_ = _other.in_angle_;
	out_angle_ = _other.out_angle_;
	attenuation_ = _other.attenuation_;
}

K::Light::Light(Light&& _other) noexcept : Component(std::move(_other))
{
	type_ = std::move(_other.type_);
	ambient_ = std::move(_other.ambient_);
	diffuse_ = std::move(_other.diffuse_);
	specular_ = std::move(_other.specular_);
	position_ = std::move(_other.position_);
	direction_ = std::move(_other.direction_);
	range_ = std::move(_other.range_);
	falloff_ = std::move(_other.falloff_);
	in_angle_ = std::move(_other.in_angle_);
	out_angle_ = std::move(_other.out_angle_);
	attenuation_ = std::move(_other.attenuation_);
}

void K::Light::_Finalize()
{
}

K::Vector4 const& K::Light::ambient() const
{
	return ambient_;
}

K::Vector4 const& K::Light::diffuse() const
{
	return diffuse_;
}

K::Vector4 const& K::Light::specular() const
{
	return specular_;
}

K::Vector3 const& K::Light::position() const
{
	return position_;
}

K::Vector3 const& K::Light::direction() const
{
	return direction_;
}

K::Vector3 const& K::Light::attenuation() const
{
	return attenuation_;
}

float K::Light::falloff() const
{
	return falloff_;
}

int K::Light::type() const
{
	return type_;
}

float K::Light::range() const
{
	return range_;
}

float K::Light::in_angle() const
{
	return in_angle_;
}

float K::Light::out_angle() const
{
	return out_angle_;
}
