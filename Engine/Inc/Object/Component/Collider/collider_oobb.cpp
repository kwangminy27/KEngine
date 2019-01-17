#include "Engine.h"
#include "collider_oobb.h"

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
#include "collider_aabb.h"

void K::ColliderOOBB::Initialize()
{
	try
	{
		mesh_ = ResourceManager::singleton()->FindMesh(COLLIDER_RECT);
		shader_ = RenderingManager::singleton()->FindShader(COLLIDER_SHADER);

		type_ = COLLIDER_TYPE::OOBB;
		group_tag_ = DEFAULT;
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ColliderOOBB::Initialize" << std::endl;
	}
}

void K::ColliderOOBB::Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }));

	absolute_info_.rotation = transform->world_rotation() * relative_info_.rotation;

	auto rotation_matrix = Matrix::CreateFromQuaternion(absolute_info_.rotation);

	absolute_info_.center = transform->world().Translation() + Vector3::Transform(relative_info_.center, rotation_matrix);
	absolute_info_.extent = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world_scaling() * 0.5f;

	auto v1 = -rotation_matrix.Right() * absolute_info_.extent.x - rotation_matrix.Up() * absolute_info_.extent.y - rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v2 = -rotation_matrix.Right() * absolute_info_.extent.x + rotation_matrix.Up() * absolute_info_.extent.y - rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v3 = rotation_matrix.Right() * absolute_info_.extent.x + rotation_matrix.Up() * absolute_info_.extent.y - rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v4 = rotation_matrix.Right() * absolute_info_.extent.x - rotation_matrix.Up() * absolute_info_.extent.y - rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v5 = -rotation_matrix.Right() * absolute_info_.extent.x - rotation_matrix.Up() * absolute_info_.extent.y + rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v6 = -rotation_matrix.Right() * absolute_info_.extent.x + rotation_matrix.Up() * absolute_info_.extent.y + rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v7 = rotation_matrix.Right() * absolute_info_.extent.x + rotation_matrix.Up() * absolute_info_.extent.y + rotation_matrix.Forward() * absolute_info_.extent.z;
	auto v8 = rotation_matrix.Right() * absolute_info_.extent.x - rotation_matrix.Up() * absolute_info_.extent.y + rotation_matrix.Forward() * absolute_info_.extent.z;

	auto x_minmax = std::minmax({ v1.x, v2.x, v3.x, v4.x, v5.x, v6.x, v7.x, v8.x }, [](float const& _p1, float const& _p2) {
		return _p1 < _p2;
	});

	auto y_minmax = std::minmax({ v1.y, v2.y, v3.y, v4.y, v5.y, v6.y, v7.y, v8.y }, [](float const& _p1, float const& _p2) {
		return _p1 < _p2;
	});

	min_ = absolute_info_.center + Vector3{ x_minmax.first, y_minmax.first, 0.f };
	max_ = absolute_info_.center + Vector3{ x_minmax.second, y_minmax.second, 0.f };
}

void K::ColliderOOBB::Render(float _time)
{
#ifdef _DEBUG
	std::shared_ptr<CameraActor> camera{};

	if (UI == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 });
	else if (DEFAULT == group_tag_)
		camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

	auto collider_position = absolute_info_.center;

	TransformConstantBuffer transform_CB{};
	transform_CB.world = Matrix::CreateScaling(CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->world_scaling()) *
		Matrix::CreateFromQuaternion(absolute_info_.rotation) *
		Matrix::CreateTranslation(absolute_info_.center);

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

K::CPTR K::ColliderOOBB::Clone() const
{
	return CPTR{ new ColliderOOBB{ *this }, [](Component* _p) {
		static_cast<ColliderOOBB*>(_p)->_Finalize();
		delete static_cast<ColliderOOBB*>(_p);
	} };
}

void K::ColliderOOBB::Serialize(InputMemoryStream& _imstream)
{
}

void K::ColliderOOBB::Serialize(OutputMemoryStream& _omstream)
{
}

K::OOBB const& K::ColliderOOBB::absolute_info() const
{
	return absolute_info_;
}

void K::ColliderOOBB::set_relative_info(AABB const& _info)
{
	relative_info_.center = _info.center;
	relative_info_.extent = _info.extent;
	absolute_info_.center = _info.center;
	absolute_info_.extent = _info.extent;
}

K::ColliderOOBB::ColliderOOBB(ColliderOOBB const& _other) : Collider(_other)
{
	relative_info_ = _other.relative_info_;
	absolute_info_ = _other.absolute_info_;
}

K::ColliderOOBB::ColliderOOBB(ColliderOOBB&& _other) noexcept : Collider(std::move(_other))
{
	relative_info_ = std::move(_other.relative_info_);
	absolute_info_ = std::move(_other.absolute_info_);
}

bool K::ColliderOOBB::_Collision(Collider* _dest, float _time)
{
	switch (_dest->type())
	{
	case COLLIDER_TYPE::POINT:
		return Collider::_CollisionOOBBToPoint(absolute_info(), static_cast<ColliderPoint*>(_dest)->absolute_info());

	case COLLIDER_TYPE::CIRCLE:
		return Collider::_CollisionOOBBToCircle(absolute_info(), static_cast<ColliderCircle*>(_dest)->absolute_info());

	case COLLIDER_TYPE::AABB:
		return Collider::_CollisionOOBBToAABB(absolute_info(), static_cast<ColliderAABB*>(_dest)->absolute_info());

	case COLLIDER_TYPE::OOBB:
		return Collider::_CollisionOOBBToOOBB(absolute_info(), static_cast<ColliderOOBB*>(_dest)->absolute_info());
	}

	return false;
}
