#include "Engine.h"
#include "component.h"

void K::Component::Initialize()
{
}

void K::Component::Input(float _time)
{
}

void K::Component::Update(float _time)
{
}

void K::Component::Collision(float _time)
{
}

void K::Component::Render(float _time)
{
}

K::APTR K::Component::owner() const
{
	return owner_.lock();
}

void K::Component::set_owner(APTR const& _actor)
{
	owner_ = _actor;
}

K::Component::Component(Component const& _other) : Tag(_other)
{
	owner_ = _other.owner_;
}

K::Component::Component(Component&& _other) noexcept : Tag(std::move(_other))
{
	owner_ = std::move(_other.owner_);
}

void K::Component::_Finalize()
{
}
