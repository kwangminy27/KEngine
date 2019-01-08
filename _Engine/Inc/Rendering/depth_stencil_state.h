#pragma once

#include "render_state.h"

namespace K
{
	class ENGINE_DLL DepthStencilState final : public RenderState
	{
		friend class RenderingManager;
	public:
		virtual void SetToShader() override;

	private:
		DepthStencilState() = default;
		DepthStencilState(DepthStencilState const& _other);
		DepthStencilState(DepthStencilState&& _other) noexcept;
		DepthStencilState& operator=(DepthStencilState const&) = default;
		DepthStencilState& operator=(DepthStencilState&&) noexcept = default;

		void _CreateState(
			BOOL _depth_enable,
			D3D11_DEPTH_WRITE_MASK _depth_write_mask,
			D3D11_COMPARISON_FUNC _depth_func,
			BOOL _stencil_enable,
			UINT8 _stencil_read_mask,
			UINT8 _stencil_write_mask,
			D3D11_DEPTH_STENCILOP_DESC const& _front_face,
			D3D11_DEPTH_STENCILOP_DESC const& _back_face);
	};
}
