#include "Engine.h"
#include "collider_aabb.h"

#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "World/world_manager.h"
#include "Object/Actor/actor.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"
#include "collider_point.h"
#include "collider_circle.h"
#include "collider_oobb.h"

void K::ColliderAABB::Initialize()
{
	try
	{
		mesh_ = ResourceManager::singleton()->FindMesh(COLLIDER_RECT);
		shader_ = RenderingManager::singleton()->FindShader(COLLIDER_SHADER);

		type_ = COLLIDER_TYPE::AABB;
		group_tag_ = DEFAULT;
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ColliderAABB::Initialize" << std::endl;
	}
}

void K::ColliderAABB::Update(float _time)
{
	auto position = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();

	absolute_info_.center = position + relative_info_.center;
	absolute_info_.extent = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world_scaling() * 0.5f;

	min_ = absolute_info_.center - absolute_info_.extent;
	max_ = absolute_info_.center + absolute_info_.extent;
}

void K::ColliderAABB::Render(float _time)
{
#ifdef _DEBUG
	std::shared_ptr<CameraActor> camera{};

	if (UI == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 });
	else if (DEFAULT == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

	auto collider_position = absolute_info_.center;

	TransformConstantBuffer transform_CB{};
	transform_CB.world = Matrix::CreateScaling(CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world_scaling()) * Matrix::CreateTranslation(collider_position);
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

K::CPTR K::ColliderAABB::Clone() const
{
	return CPTR{ new ColliderAABB{ *this }, [](Component* _p) {
		static_cast<ColliderAABB*>(_p)->_Finalize();
		delete static_cast<ColliderAABB*>(_p);
	} };
}

void K::ColliderAABB::Serialize(InputMemoryStream& _imstream)
{
}

void K::ColliderAABB::Serialize(OutputMemoryStream& _omstream)
{
}

K::AABB const& K::ColliderAABB::absolute_info() const
{
	return absolute_info_;
}

void K::ColliderAABB::set_relative_info(AABB const& _info)
{
	relative_info_ = _info;
	absolute_info_ = _info;
}

K::ColliderAABB::ColliderAABB(ColliderAABB const& _other) : Collider(_other)
{
	relative_info_ = _other.relative_info_;
	absolute_info_ = _other.absolute_info_;
}

K::ColliderAABB::ColliderAABB(ColliderAABB&& _other) noexcept : Collider(std::move(_other))
{
	relative_info_ = std::move(_other.relative_info_);
	absolute_info_ = std::move(_other.absolute_info_);
}

bool K::ColliderAABB::_Collision(Collider* _dest, float _time)
{
	switch (_dest->type())
	{
	case COLLIDER_TYPE::POINT:
		return Collider::_CollisionAABBToPoint(absolute_info(), static_cast<ColliderPoint*>(_dest)->absolute_info());

	case COLLIDER_TYPE::CIRCLE:
		return Collider::_CollisionAABBToCircle(absolute_info(), static_cast<ColliderCircle*>(_dest)->absolute_info());

	case COLLIDER_TYPE::AABB:
		return Collider::_CollisionAABBToAABB(absolute_info(), static_cast<ColliderAABB*>(_dest)->absolute_info());

	case COLLIDER_TYPE::OOBB:
		return Collider::_CollisionOOBBToAABB(static_cast<ColliderOOBB*>(_dest)->absolute_info(), absolute_info());
	}

	return false;
}
