#include "Engine.h"
#include "transform.h"

#include "Rendering/rendering_manager.h"
#include "World/world_manager.h"
#include "Object/Actor/camera_actor.h"

void K::Transform::Initialize()
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
		std::cout << "Transform::Initialize" << std::endl;
	}
}

void K::Transform::Update(float _time)
{
	if (!dirty_flag_)
		return;

	local_ = Matrix::CreateScaling(local_scaling_) * Matrix::CreateFromQuaternion(local_rotation_) * Matrix::CreateTranslation(local_translation_);

	parent_ = Matrix::Identity;

	world_scaling_ = local_scaling_;
	world_rotation_ = local_rotation_;
	world_translation_ = local_translation_;

	if (parent_flag_ & static_cast<int>(PARENT_FLAG::SCALING))
	{
		parent_ *= Matrix::CreateScaling(parent_scaling_);

		world_scaling_ *= parent_scaling_;
	}

	if (parent_flag_ & static_cast<int>(PARENT_FLAG::ROTATION))
	{
		parent_ *= Matrix::CreateFromQuaternion(parent_rotation_);

		world_rotation_ *= parent_rotation_;
	}

	if (parent_flag_ & static_cast<int>(PARENT_FLAG::TRANSLATION))
	{
		parent_ *= Matrix::CreateTranslation(parent_translation_);

		world_translation_ += parent_translation_;
	}

	world_ = local_ * parent_;

	for (auto const& child : owner()->child_list())
	{
		auto const& child_transform = CPTR_CAST<Transform>(child->FindComponent(TAG{ TRANSFORM, 0 }));

		child_transform->set_parent_scaling(world_scaling_);
		child_transform->set_parent_rotation(world_rotation_);
		child_transform->set_parent_translation(world_translation_);

		child_transform->set_dirty_flag(true);
	}

	dirty_flag_ = false;
}

K::CPTR K::Transform::Clone() const
{
	return CPTR{ new Transform{ *this }, [](Component* _p) {
		static_cast<Transform*>(_p)->_Finalize();
		delete static_cast<Transform*>(_p);
	} };
}

void K::Transform::Serialize(InputMemoryStream& _imstream)
{
	_imstream.Serialize(parent_flag_);

	_imstream.Serialize(local_scaling_.x);
	_imstream.Serialize(local_scaling_.y);
	_imstream.Serialize(local_scaling_.z);
	_imstream.Serialize(parent_scaling_.x);
	_imstream.Serialize(parent_scaling_.y);
	_imstream.Serialize(parent_scaling_.z);

	_imstream.Serialize(local_rotation_.x);
	_imstream.Serialize(local_rotation_.y);
	_imstream.Serialize(local_rotation_.z);
	_imstream.Serialize(local_rotation_.w);
	_imstream.Serialize(parent_rotation_.x);
	_imstream.Serialize(parent_rotation_.y);
	_imstream.Serialize(parent_rotation_.z);
	_imstream.Serialize(parent_rotation_.w);

	_imstream.Serialize(local_translation_.x);
	_imstream.Serialize(local_translation_.y);
	_imstream.Serialize(local_translation_.z);
	_imstream.Serialize(parent_translation_.x);
	_imstream.Serialize(parent_translation_.y);
	_imstream.Serialize(parent_translation_.z);
}

void K::Transform::Serialize(OutputMemoryStream& _omstream)
{
	_omstream.Serialize(parent_flag_);

	_omstream.Serialize(local_scaling_.x);
	_omstream.Serialize(local_scaling_.y);
	_omstream.Serialize(local_scaling_.z);
	_omstream.Serialize(parent_scaling_.x);
	_omstream.Serialize(parent_scaling_.y);
	_omstream.Serialize(parent_scaling_.z);

	_omstream.Serialize(local_rotation_.x);
	_omstream.Serialize(local_rotation_.y);
	_omstream.Serialize(local_rotation_.z);
	_omstream.Serialize(local_rotation_.w);
	_omstream.Serialize(parent_rotation_.x);
	_omstream.Serialize(parent_rotation_.y);
	_omstream.Serialize(parent_rotation_.z);
	_omstream.Serialize(parent_rotation_.w);

	_omstream.Serialize(local_translation_.x);
	_omstream.Serialize(local_translation_.y);
	_omstream.Serialize(local_translation_.z);
	_omstream.Serialize(parent_translation_.x);
	_omstream.Serialize(parent_translation_.y);
	_omstream.Serialize(parent_translation_.z);
}

void K::Transform::UpdateConstantBuffer()
{
	std::shared_ptr<CameraActor> camera{};

	if (true == owner()->ui_flag())
		camera = WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 });
	else
		camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

	TransformConstantBuffer transform_CB{};
	transform_CB.world = world_;
	transform_CB.view = camera->view();
	transform_CB.projection = camera->projection();
	transform_CB.WVP = transform_CB.world * transform_CB.view * transform_CB.projection;

	transform_CB.world = transform_CB.world.Transpose();
	transform_CB.view = transform_CB.view.Transpose();
	transform_CB.projection = transform_CB.projection.Transpose();
	transform_CB.WVP = transform_CB.WVP.Transpose();

	RenderingManager::singleton()->UpdateConstantBuffer(TRANSFORM, &transform_CB);
}

bool K::Transform::dirty_flag() const
{
	return dirty_flag_;
}

uint8_t K::Transform::parent_flag() const
{
	return parent_flag_;
}

K::Vector3 const& K::Transform::local_scaling() const
{
	return local_scaling_;
}

K::Vector3 const& K::Transform::parent_scaling() const
{
	return parent_scaling_;
}

K::Vector3 const& K::Transform::world_scaling() const
{
	return world_scaling_;
}

K::Quaternion const& K::Transform::local_rotation() const
{
	return local_rotation_;
}

K::Quaternion const& K::Transform::parent_rotation() const
{
	return parent_rotation_;
}

K::Quaternion const& K::Transform::world_rotation() const
{
	return world_rotation_;
}

K::Vector3 const& K::Transform::local_translation() const
{
	return local_translation_;
}

K::Vector3 const& K::Transform::parent_translation() const
{
	return parent_translation_;
}

K::Vector3 const& K::Transform::world_translation() const
{
	return world_translation_;
}

K::Matrix const& K::Transform::local() const
{
	return local_;
}

K::Matrix const& K::Transform::parent() const
{
	return parent_;
}

K::Matrix const& K::Transform::world() const
{
	return world_;
}

void K::Transform::set_dirty_flag(bool _flag)
{
	dirty_flag_ = _flag;
}

void K::Transform::set_parent_flag(uint8_t _flag)
{
	parent_flag_ = _flag;
}

void K::Transform::set_local_scaling(Vector3 const& _v)
{
	local_scaling_ = _v;

	dirty_flag_ = true;
}

void K::Transform::set_parent_scaling(Vector3 const& _v)
{
	parent_scaling_ = _v;

	dirty_flag_ = true;
}

void K::Transform::set_local_rotation(Quaternion const& _q)
{
	local_rotation_ = _q;

	dirty_flag_ = true;
}

void K::Transform::set_parent_rotation(Quaternion const& _q)
{
	parent_rotation_ = _q;

	dirty_flag_ = true;
}

void K::Transform::set_local_translation(Vector3 const & _v)
{
	local_translation_ = _v;

	dirty_flag_ = true;
}

void K::Transform::set_parent_translation(Vector3 const & _v)
{
	parent_translation_ = _v;

	dirty_flag_ = true;
}

K::Transform::Transform(Transform const& _other) : Component(_other)
{
	dirty_flag_ = _other.dirty_flag_;
	parent_flag_ = _other.parent_flag_;

	local_scaling_ = _other.local_scaling_;
	parent_scaling_ = _other.parent_scaling_;
	world_scaling_ = _other.world_scaling_;

	local_rotation_ = _other.local_rotation_;
	parent_rotation_ = _other.parent_rotation_;
	world_rotation_ = _other.world_rotation_;

	local_translation_ = _other.local_translation_;
	parent_translation_ = _other.parent_translation_;
	world_translation_ = _other.world_translation_;

	local_ = _other.local_;
	parent_ = _other.parent_;
	world_ = _other.world_;
}

K::Transform::Transform(Transform&& _other) noexcept : Component(std::move(_other))
{
	dirty_flag_ = std::move(_other.dirty_flag_);
	parent_flag_ = std::move(_other.parent_flag_);

	local_scaling_ = std::move(_other.local_scaling_);
	parent_scaling_ = std::move(_other.parent_scaling_);
	world_scaling_ = std::move(_other.world_scaling_);

	local_rotation_ = std::move(_other.local_rotation_);
	parent_rotation_ = std::move(_other.parent_rotation_);
	world_rotation_ = std::move(_other.world_rotation_);

	local_translation_ = std::move(_other.local_translation_);
	parent_translation_ = std::move(_other.parent_translation_);
	world_translation_ = std::move(_other.world_translation_);

	local_ = std::move(_other.local_);
	parent_ = std::move(_other.parent_);
	world_ = std::move(_other.world_);
}

void K::Transform::_Finalize()
{
}
