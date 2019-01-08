#include "Engine.h"
#include "collider.h"

#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"

void K::Collider::Render(float _time)
{
#ifdef _DEBUG
	if (collided_collider_list_.empty())
		color_ = DirectX::Colors::Green.v;
	else
		color_ = DirectX::Colors::Red.v;

	RenderingManager::singleton()->UpdateConstantBuffer(COLLIDER, &color_);

	shader_->SetToShader();
	mesh_->Render();
#endif
}

void K::Collider::AddCallback(std::function<void(Collider*, Collider*, float)> const& _callback, COLLISION_CALLBACK_TYPE _type)
{
	callback_list_array_.at(static_cast<int>(_type)).push_back(_callback);
}

K::COLLIDER_TYPE K::Collider::type() const
{
	return type_;
}

void K::Collider::set_group_tag(std::string const& _tag)
{
	group_tag_ = _tag;
}

K::Collider::Collider(Collider const& _other) : Component(_other)
{
	color_ = _other.color_;
	mesh_ = _other.mesh_;
	shader_ = _other.shader_;

	min_ = _other.min_;
	max_ = _other.max_;
	type_ = _other.type_;
	group_tag_ = _other.group_tag_;
	section_idx_list_ = _other.section_idx_list_;
	collided_collider_list_ = _other.collided_collider_list_;
	callback_list_array_ = _other.callback_list_array_;
}

K::Collider::Collider(Collider&& _other) noexcept : Component(std::move(_other))
{
	color_ = std::move(_other.color_);
	mesh_ = std::move(_other.mesh_);
	shader_ = std::move(_other.shader_);

	min_ = std::move(_other.min_);
	max_ = std::move(_other.max_);
	type_ = std::move(_other.type_);
	group_tag_ = std::move(_other.group_tag_);
	section_idx_list_ = std::move(_other.section_idx_list_);
	collided_collider_list_ = std::move(_other.collided_collider_list_);
	callback_list_array_ = std::move(_other.callback_list_array_);
}

void K::Collider::_Finalize()
{
	for (auto iter = collided_collider_list_.begin(); iter != collided_collider_list_.end(); ++iter)
	{
		(*iter)->_OnCollisionLeave(this, 0.f);
		_OnCollisionLeave(*iter, 0.f);

		(*iter)->_RemoveCollidedCollider(this);
	}
}

bool K::Collider::_CollisionPointToPoint(Vector3 const& _src, Vector3 const& _dest)
{
	return _src == _dest;
}

bool K::Collider::_CollisionCircleToPoint(Circle const& _src, Vector3 const& _dest)
{
	return (_dest - _src.center).Length() <= _src.radius;
}

bool K::Collider::_CollisionCircleToCircle(Circle const& _src, Circle const& _dest)
{
	return (_dest.center - _src.center).Length() <= _src.radius + _dest.radius;
}

bool K::Collider::_CollisionAABBToPoint(AABB const& _src, Vector3 const& _dest)
{
	auto src_min = _src.center - _src.extent;
	auto src_max = _src.center + _src.extent;

	if (_dest.x < src_min.x)
		return false;
	else if (_dest.y < src_min.y)
		return false;
	else if (_dest.z < src_min.z)
		return false;
	else if (_dest.x > src_max.x)
		return false;
	else if (_dest.y > src_max.y)
		return false;
	else if (_dest.z > src_max.z)
		return false;

	return true;
}

bool K::Collider::_CollisionAABBToCircle(AABB const& _src, Circle const& _dest)
{
	auto src_min = _src.center - _src.extent;
	auto src_max = _src.center + _src.extent;

	auto closest_point = _dest.center;
	closest_point.Clamp(src_min, src_max);

	return Vector3::Distance(closest_point, _dest.center) <= _dest.radius;
}

bool K::Collider::_CollisionAABBToAABB(AABB const& _src, AABB const& _dest)
{
	auto src_min = _src.center - _src.extent;
	auto src_max = _src.center + _src.extent;

	auto dest_min = _dest.center - _dest.extent;
	auto dest_max = _dest.center + _dest.extent;

	if (dest_max.x < src_min.x)
		return false;
	else if (dest_max.y < src_min.y)
		return false;
	else if (dest_max.z < src_min.z)
		return false;
	else if (dest_min.x > src_max.x)
		return false;
	else if (dest_min.y > src_max.y)
		return false;
	else if (dest_min.z > src_max.z)
		return false;

	return true;
}

bool K::Collider::_CollisionOOBBToPoint(OOBB const& _src, Vector3 const& _dest)
{
	auto rotation_matrix = Matrix::CreateFromQuaternion(_src.rotation);
	auto right = rotation_matrix.Right();
	auto up = rotation_matrix.Up();
	auto forward = rotation_matrix.Forward();

	if (right.Dot(_dest) - right.Dot(_src.center + right * _src.extent) > 0.f)
		return false;
	if (-right.Dot(_dest) + right.Dot(_src.center - right * _src.extent) > 0.f)
		return false;
	if (up.Dot(_dest) - up.Dot(_src.center + up * _src.extent) > 0.f)
		return false;
	if (-up.Dot(_dest) + up.Dot(_src.center - up * _src.extent) > 0.f)
		return false;
	if (forward.Dot(_dest) - forward.Dot(_src.center + forward * _src.extent) > 0.f)
		return false;
	if (-forward.Dot(_dest) + forward.Dot(_src.center - forward * _src.extent) > 0.f)
		return false;

	return true;
}

bool K::Collider::_CollisionOOBBToCircle(OOBB const& _src, Circle const& _dest)
{
	AABB aabb{};
	aabb.center = _src.center;
	aabb.extent = _src.extent;

	auto rotation_matrix = Matrix::CreateFromQuaternion(_src.rotation);

	Circle circle{};
	circle.center = _src.center + Vector3::TransformNormal(_dest.center - _src.center, rotation_matrix.Inverse());
	circle.radius = _dest.radius;

	return _CollisionAABBToCircle(aabb, circle);
}

bool K::Collider::_CollisionOOBBToAABB(OOBB const& _src, AABB const& _dest)
{
	OOBB oobb{};
	oobb.center = _dest.center;
	oobb.extent = _dest.extent;
	oobb.rotation = Quaternion::Identity;

	return _CollisionOOBBToOOBB(_src, oobb);
}

bool K::Collider::_CollisionOOBBToOOBB(OOBB const& _src, OOBB const& _dest)
{
	auto src_rotation_matrix = Matrix::CreateFromQuaternion(_src.rotation);
	auto src_right = src_rotation_matrix.Right();
	auto src_up = src_rotation_matrix.Up();
	auto src_forward = src_rotation_matrix.Forward();

	auto dest_rotation_matrix = Matrix::CreateFromQuaternion(_dest.rotation);
	auto dest_right = dest_rotation_matrix.Right();
	auto dest_up = dest_rotation_matrix.Up();
	auto dest_forward = dest_rotation_matrix.Forward();

	auto src_length_on_dest_right = fabsf((src_right * _src.extent.x).Dot(dest_right)) + fabsf((src_up * _src.extent.y).Dot(dest_right)) + fabsf((src_forward * _src.extent.z).Dot(dest_right));
	auto src_length_on_dest_up = fabsf((src_right * _src.extent.x).Dot(dest_up)) + fabsf((src_up * _src.extent.y).Dot(dest_up)) + fabsf((src_forward * _src.extent.z).Dot(dest_up));
	auto src_length_on_dest_forward = fabsf((src_right * _src.extent.x).Dot(dest_forward)) + fabsf((src_up * _src.extent.y).Dot(dest_forward)) + fabsf((src_forward * _src.extent.z).Dot(dest_forward));

	auto dest_length_on_src_right = fabsf((dest_right * _dest.extent.x).Dot(src_right)) + fabsf((dest_up * _dest.extent.y).Dot(src_right)) + fabsf((dest_forward * _dest.extent.z).Dot(src_right));
	auto dest_length_on_src_up = fabsf((dest_right * _dest.extent.x).Dot(src_up)) + fabsf((dest_up * _dest.extent.y).Dot(src_up)) + fabsf((dest_forward * _dest.extent.z).Dot(src_up));
	auto dest_length_on_src_forward = fabsf((dest_right * _dest.extent.x).Dot(src_forward)) + fabsf((dest_up * _dest.extent.y).Dot(src_forward)) + fabsf((dest_forward * _dest.extent.z).Dot(src_forward));

	float src_length{};
	float dest_length{};
	float src_to_dest_length{};

	auto src_to_dest = _dest.center - _src.center;

	src_length = _src.extent.x;
	dest_length = dest_length_on_src_right;
	src_to_dest_length = fabsf(src_to_dest.Dot(src_right));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	src_length = _src.extent.y;
	dest_length = dest_length_on_src_up;
	src_to_dest_length = fabsf(src_to_dest.Dot(src_up));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	src_length = _src.extent.z;
	dest_length = dest_length_on_src_forward;
	src_to_dest_length = fabsf(src_to_dest.Dot(src_forward));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	src_length = src_length_on_dest_right;
	dest_length = _dest.extent.x;
	src_to_dest_length = fabsf(src_to_dest.Dot(dest_right));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	src_length = src_length_on_dest_up;
	dest_length = _dest.extent.y;
	src_to_dest_length = fabsf(src_to_dest.Dot(dest_up));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	src_length = src_length_on_dest_forward;
	dest_length = _dest.extent.z;
	src_to_dest_length = fabsf(src_to_dest.Dot(dest_forward));

	if (src_length + dest_length < src_to_dest_length)
		return false;

	Vector3 axis[9]{};
	axis[0] = src_right.Cross(dest_right);
	axis[1] = src_right.Cross(dest_up);
	axis[2] = src_right.Cross(dest_forward);
	axis[3] = src_up.Cross(dest_right);
	axis[4] = src_up.Cross(dest_up);
	axis[5] = src_up.Cross(dest_forward);
	axis[6] = src_forward.Cross(dest_right);
	axis[7] = src_forward.Cross(dest_up);
	axis[8] = src_forward.Cross(dest_forward);

	for (auto i = 0; i < 9; ++i)
	{
		src_length = fabsf((src_right * _src.extent.x).Dot(axis[i])) + fabsf((src_up * _src.extent.y).Dot(axis[i])) + fabsf((src_forward * _src.extent.z).Dot(axis[i]));
		dest_length = fabsf((dest_right * _dest.extent.x).Dot(axis[i])) + fabsf((dest_up * _dest.extent.y).Dot(axis[i])) + fabsf((dest_forward * _dest.extent.z).Dot(axis[i]));
		src_to_dest_length = fabsf(src_to_dest.Dot(axis[i]));

		if (src_length + dest_length < src_to_dest_length)
			return false;
	}

	return true;
}

void K::Collider::_OnCollisionEnter(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::ENTER)))
		callback(this, _dest, _time);
}

void K::Collider::_OnCollisionStay(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::STAY)))
		callback(this, _dest, _time);
}

void K::Collider::_OnCollisionLeave(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::LEAVE)))
		callback(this, _dest, _time);
}

void K::Collider::_AddSectionIdx(uint32_t _idx)
{
	section_idx_list_.push_back(_idx);
}

void K::Collider::_UpdateCollidedCollider(float _time)
{
	for (auto iter = collided_collider_list_.begin(); iter != collided_collider_list_.end();)
	{
		if (group_tag_ != (*iter)->group_tag_)
		{
			(*iter)->_OnCollisionLeave(this, _time);
			_OnCollisionLeave(*iter, _time);
			
			(*iter)->_RemoveCollidedCollider(this);
			iter = collided_collider_list_.erase(iter);

			continue;
		}

		bool is_overlapped{};

		for (auto const& src_section_idx : section_idx_list_)
		{
			for (auto const& dest_section_idx : (*iter)->section_idx_list_)
			{
				if (src_section_idx == dest_section_idx)
					is_overlapped = true;
			}
		}

		if (false == is_overlapped)
		{
			(*iter)->_OnCollisionLeave(this, _time);
			_OnCollisionLeave(*iter, _time);

			(*iter)->_RemoveCollidedCollider(this);
			iter = collided_collider_list_.erase(iter);

			continue;
		}

		++iter;
	}
}

bool K::Collider::_IsCollidedCollider(Collider* _dest)
{
	auto iter = std::find(collided_collider_list_.begin(), collided_collider_list_.end(), _dest);

	if (iter == collided_collider_list_.end())
		return false;

	return true;
}

void K::Collider::_AddCollidedCollider(Collider* _dest)
{
	collided_collider_list_.push_back(_dest);
}

void K::Collider::_RemoveCollidedCollider(Collider* _dest)
{
	collided_collider_list_.remove(_dest);
}
