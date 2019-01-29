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
	};
}
