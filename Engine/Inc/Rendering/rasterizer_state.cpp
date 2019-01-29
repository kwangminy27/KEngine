#include "Engine.h"
#include "rasterizer_state.h"

void K::RasterizerState::SetState()
{
}

void K::RasterizerState::ResetState()
{
}

K::RasterizerState::RasterizerState(RasterizerState const& _other) : RenderState(_other)
{
}

K::RasterizerState::RasterizerState(RasterizerState&& _other) noexcept : RenderState(std::move(_other))
{
}
