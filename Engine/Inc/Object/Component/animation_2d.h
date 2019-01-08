#pragma once

#include "component.h"

namespace K
{
	class ENGINE_DLL Animation2D final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Update(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void UpdateConstantBuffer(float _time);

		static std::shared_ptr<ANIMATION_2D_CLIP_DESC> animation_2d_clip_desc_dummy_;

	private:
		Animation2D() = default;
		Animation2D(Animation2D const& _other);
		Animation2D(Animation2D&& _other) noexcept;
		Animation2D& operator=(Animation2D const&) = delete;
		Animation2D& operator=(Animation2D&&) noexcept = default;

		virtual void _Finalize() override;

		float elapsed_time_{};
		uint16_t frame_idx_{};
		std::shared_ptr<ANIMATION_2D_CLIP_DESC> current_clip_{};
		std::shared_ptr<ANIMATION_2D_CLIP_DESC> default_clip_{};
		std::unordered_map<std::string, std::shared_ptr<ANIMATION_2D_CLIP_DESC>> animation_2d_clip_map_{};
	};
}
