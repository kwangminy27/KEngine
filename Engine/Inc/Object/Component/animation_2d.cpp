#include "Engine.h"
#include "animation_2d.h"

#include "Rendering/rendering_manager.h"
#include "Object/Actor/actor.h"

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> K::Animation2D::animation_2d_clip_desc_dummy_{};

void K::Animation2D::Initialize()
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
		std::cout << "Animation2D::Initialize" << std::endl;
	}
}

void K::Animation2D::Update(float _time)
{
	elapsed_time_ += _time;

	float interval = current_clip_->completion_time / current_clip_->frame_vector.size();

	while (elapsed_time_ >= interval)
	{
		elapsed_time_ -= interval;

		++frame_idx_;

		if (frame_idx_ == current_clip_->frame_vector.size())
		{
			frame_idx_ = 0;

			switch (current_clip_->option)
			{
			case ANIMATION_OPTION::DESTROY:
				owner()->set_tag_state(TAG_STATE::DEAD);
				break;
			}
		}
	}
}

K::CPTR K::Animation2D::Clone() const
{
	return CPTR{ new Animation2D{ *this }, [](Component* _p) {
		static_cast<Animation2D*>(_p)->_Finalize();
		delete static_cast<Animation2D*>(_p);
	} };
}

void K::Animation2D::Serialize(InputMemoryStream& _imstream)
{
}

void K::Animation2D::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Animation2D::UpdateConstantBuffer(float _time)
{
	ANIMATION_2D_FRAME_DESC animation_2d_frame_desc{};

	switch (current_clip_->type)
	{
	case ANIMATION_2D_TYPE::FRAME:
		animation_2d_frame_desc.LT = { 0.f, 0.f };
		animation_2d_frame_desc.RB = { 1.f, 1.f };
		break;

	case ANIMATION_2D_TYPE::ATLAS:
		animation_2d_frame_desc.LT.x = current_clip_->frame_vector.at(frame_idx_).LT.x / current_clip_->width;
		animation_2d_frame_desc.LT.y = current_clip_->frame_vector.at(frame_idx_).LT.y / current_clip_->height;
		animation_2d_frame_desc.RB.x = current_clip_->frame_vector.at(frame_idx_).RB.x / current_clip_->width;
		animation_2d_frame_desc.RB.y = current_clip_->frame_vector.at(frame_idx_).RB.y / current_clip_->height;
		break;
	}

	RenderingManager::singleton()->UpdateConstantBuffer(ANIMATION_2D, &animation_2d_frame_desc);
}

K::Animation2D::Animation2D(Animation2D const& _other) : Component(_other)
{
}

K::Animation2D::Animation2D(Animation2D&& _other) noexcept : Component(std::move(_other))
{
}

void K::Animation2D::_Finalize()
{
}
