#pragma once

#include "render_state.h"

namespace K
{
	class ENGINE_DLL BlendState final : public RenderState
	{
		friend class RenderingManager;
	public:
		virtual void SetState() override;
		virtual void ResetState() override;

	private:
		BlendState() = default;
		BlendState(BlendState const& _other);
		BlendState(BlendState&& _other) noexcept;
		BlendState& operator=(BlendState const&) = default;
		BlendState& operator=(BlendState&&) noexcept = default;

		void _CreateState(
			bool _alpha_to_coverage_enable,
			bool _independent_blend_enable,
			std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector);

		// TODO: SAMPLE_MASK 0으로 두면 SRC랑 DEST에서 샘플링 안해옴 그래서 클리어 색상만 나왔던 것임
		uint32_t sample_mask_{ 0xffffffff };
		uint32_t old_sample_mask_;
		std::array<float, 4> blend_factor_array_;
		std::array<float, 4> old_blend_factor_array_;
	};
}
