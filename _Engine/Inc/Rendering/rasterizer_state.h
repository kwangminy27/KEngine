#pragma once

#include "render_state.h"

namespace K
{
	class ENGINE_DLL RasterizerState final : public RenderState
	{
		friend class RenderingManager;
	public:
		virtual void SetState() override;
		virtual void ResetState() override;

	private:
		RasterizerState() = default;
		RasterizerState(RasterizerState const& _other);
		RasterizerState(RasterizerState&& _other) noexcept;
		RasterizerState& operator=(RasterizerState const&) = default;
		RasterizerState& operator=(RasterizerState&&) noexcept = default;

		void _CreateState(
			D3D11_FILL_MODE _fill_mode,
			D3D11_CULL_MODE _cull_mode,
			bool _front_counter_clock_wise,
			int _depth_bias,
			float _depth_bias_clamp,
			float _slope_scale_depth_bias,
			bool _depth_clip_enable,
			bool _scissor_enable,
			bool _multi_sample_enable,
			bool _anti_aliased_line_enable);
	};
}
