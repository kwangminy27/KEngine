#pragma once

namespace K
{
	class ENGINE_DLL RenderState
	{
		friend class RenderingManager;
	public:
		virtual void SetState() = 0;
		virtual void ResetState() = 0;

	protected:
		RenderState() = default;
		RenderState(RenderState const& _other);
		RenderState(RenderState&& _other) noexcept;
		RenderState& operator=(RenderState const&) = default;
		RenderState& operator=(RenderState&&) noexcept = default;
		virtual ~RenderState() = default;

		Microsoft::WRL::ComPtr<ID3D11DeviceChild> render_state_{};
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> old_render_state_{};
	};
}
