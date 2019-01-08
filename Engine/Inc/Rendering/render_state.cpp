#include "Engine.h"
#include "render_state.h"

K::RenderState::RenderState(RenderState const& _other)
{
	*this = _other;
}

K::RenderState::RenderState(RenderState&& _other) noexcept
{
	*this = std::move(_other);
}
