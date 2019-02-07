#include "Engine.h"
#include "camera.h"

#include "Object/Actor/actor.h"
#include "Object/Component/transform.h"

void K::Camera::Initialize()
{
	try
	{
		CreateProjection(45.f, static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT), 0.001f, 1000.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Camera::Initialize" << std::endl;
	}
}

K::CPTR K::Camera::Clone() const
{
	return CPTR{ new Camera{ *this }, [](Component* _p) {
		static_cast<Camera*>(_p)->_Finalize();
		delete static_cast<Camera*>(_p);
	} };
}

void K::Camera::Serialize(InputMemoryStream& _imstream)
{
}

void K::Camera::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Camera::CreateLookAt(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up)
{
	view_ = Matrix::CreateLookAt(_eye, _focus, _up);
}

void K::Camera::CreateLookTo(Vector3 const& _position, Vector3 const& _direction, Vector3 const& _up)
{
	view_ = Matrix::CreateLookTo(_position, _direction, _up);
}

void K::Camera::CreateProjection(float _width, float _height, float _near, float _far)
{
	projection_ = Matrix::CreateOrthographic(_width, _height, _near, _far);
	far_ = _far;
}

void K::Camera::CreateProjection(float _fov_angle, float _width, float _height, float _near, float _far)
{
	assert(_near != 0.f);

	projection_ = Matrix::CreatePerspectiveFieldOfView(_fov_angle, _width / _height, _near, _far);
	far_ = _far;
}

float K::Camera::GetProjectionFar()
{
	return far_;
}

K::Matrix const& K::Camera::view() const
{
	return view_;
}

K::Matrix const& K::Camera::projection() const
{
	return projection_;
}

K::Camera::Camera(Camera const& _other) : Component(_other)
{
	view_ = _other.view_;
	projection_ = _other.projection_;
}

K::Camera::Camera(Camera&& _other) noexcept : Component(std::move(_other))
{
	view_ = std::move(_other.view_);
	projection_ = std::move(_other.projection_);
}

void K::Camera::_Finalize()
{
}
