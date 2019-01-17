#include "Engine.h"
#include "collider_point.h"

#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "World/world_manager.h"
#include "Object/Actor/actor.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/camera.h"
#include "collider_circle.h"
#include "collider_aabb.h"
#include "collider_oobb.h"

void K::ColliderPoint::Initialize()
{
	try
	{
		mesh_ = ResourceManager::singleton()->FindMesh(COLLIDER_RECT);
		shader_ = RenderingManager::singleton()->FindShader(COLLIDER_SHADER);

		type_ = COLLIDER_TYPE::POINT;
		group_tag_ = DEFAULT;
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ColliderPoint::Initialize" << std::endl;
	}
}

void K::ColliderPoint::Update(float _time)
{
	auto position = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();

	absolute_info_ = position + relative_info_;

	min_ = absolute_info_;
	max_ = absolute_info_;
}

void K::ColliderPoint::Render(float _time)
{
#ifdef _DEBUG
	std::shared_ptr<CameraActor> camera{};

	if (UI == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 });
	else if(DEFAULT == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

	auto collider_position = min_;

	TransformConstantBuffer transform_CB{};
	transform_CB.world = Matrix::CreateTranslation(collider_position);
	transform_CB.view = camera->view();
	transform_CB.projection = camera->projection();
	transform_CB.WV = transform_CB.world * transform_CB.view;
	transform_CB.WVP = transform_CB.world * transform_CB.view * transform_CB.projection;
	transform_CB.projection_Inv = transform_CB.projection.Inverse();

	transform_CB.world = transform_CB.world.Transpose();
	transform_CB.view = transform_CB.view.Transpose();
	transform_CB.projection = transform_CB.projection.Transpose();
	transform_CB.WV = transform_CB.WV.Transpose();
	transform_CB.WVP = transform_CB.WVP.Transpose();
	transform_CB.projection_Inv = transform_CB.projection_Inv.Transpose();

	RenderingManager::singleton()->UpdateConstantBuffer(TRANSFORM, &transform_CB);

	Collider::Render(_time);
#endif
}

K::CPTR K::ColliderPoint::Clone() const
{
	return CPTR{ new ColliderPoint{ *this }, [](Component* _p) {
		static_cast<ColliderPoint*>(_p)->_Finalize();
		delete static_cast<ColliderPoint*>(_p);
	} };
}

void K::ColliderPoint::Serialize(InputMemoryStream& _imstream)
{
}

void K::ColliderPoint::Serialize(OutputMemoryStream& _omstream)
{
}

K::Vector3 const& K::ColliderPoint::absolute_info() const
{
	return absolute_info_;
}

void K::ColliderPoint::set_relative_info(Vector3 const& _info)
{
	relative_info_ = _info;
	absolute_info_ = _info;
}

K::ColliderPoint::ColliderPoint(ColliderPoint const& _other) : Collider(_other)
{
	relative_info_ = _other.relative_info_;
	absolute_info_ = _other.absolute_info_;
}

K::ColliderPoint::ColliderPoint(ColliderPoint&& _other) noexcept : Collider(std::move(_other))
{
	relative_info_ = std::move(_other.relative_info_);
	absolute_info_ = std::move(_other.absolute_info_);
}

bool K::ColliderPoint::_Collision(Collider* _dest, float _time)
{
	switch (_dest->type())
	{
	case COLLIDER_TYPE::POINT:
		return Collider::_CollisionPointToPoint(absolute_info(), static_cast<ColliderPoint*>(_dest)->absolute_info());

	case COLLIDER_TYPE::CIRCLE:
		return Collider::_CollisionCircleToPoint(static_cast<ColliderCircle*>(_dest)->absolute_info(), absolute_info());

	case COLLIDER_TYPE::AABB:
		return Collider::_CollisionAABBToPoint(static_cast<ColliderAABB*>(_dest)->absolute_info(), absolute_info());

	case COLLIDER_TYPE::OOBB:
		return Collider::_CollisionOOBBToPoint(static_cast<ColliderOOBB*>(_dest)->absolute_info(), absolute_info());
	}

	return false;
}
