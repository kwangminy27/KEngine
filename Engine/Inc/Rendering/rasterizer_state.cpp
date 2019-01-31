#include "Engine.h"
#include "rasterizer_state.h"

#include "device_manager.h"

void K::RasterizerState::SetState()
{
	auto const& context = DeviceManager::singleton()->context();

	context->RSGetState(reinterpret_cast<ID3D11RasterizerState**>(old_render_state_.ReleaseAndGetAddressOf()));
	context->RSSetState(reinterpret_cast<ID3D11RasterizerState*>(render_state_.Get()));
}

void K::RasterizerState::ResetState()
{
	auto const& context = DeviceManager::singleton()->context();

	context->RSSetState(reinterpret_cast<ID3D11RasterizerState*>(old_render_state_.Get()));
}

K::RasterizerState::RasterizerState(RasterizerState const& _other) : RenderState(_other)
{
}

K::RasterizerState::RasterizerState(RasterizerState&& _other) noexcept : RenderState(std::move(_other))
{
}

void K::RasterizerState::_CreateState(
	D3D11_FILL_MODE _fill_mode,
	D3D11_CULL_MODE _cull_mode,
	bool _front_counter_clock_wise,
	int _depth_bias,
	float _depth_bias_clamp,
	float _slope_scale_depth_bias,
	bool _depth_clip_enable,
	bool _scissor_enable,
	bool _multi_sample_enable,
	bool _anti_aliased_line_enable)
{
	auto const& device = DeviceManager::singleton()->device();

	D3D11_RASTERIZER_DESC drd{};
	drd.FillMode = _fill_mode;
	drd.CullMode = _cull_mode;
	drd.FrontCounterClockwise = _front_counter_clock_wise;
	drd.DepthBias = _depth_bias;
	drd.DepthBiasClamp = _depth_bias_clamp;
	drd.SlopeScaledDepthBias = _slope_scale_depth_bias;
	drd.DepthClipEnable = _depth_clip_enable;
	drd.ScissorEnable = _scissor_enable;
	drd.MultisampleEnable = _multi_sample_enable;
	drd.AntialiasedLineEnable = _anti_aliased_line_enable;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_{};
	device->CreateRasterizerState(&drd, &rasterizer_state_);

	ThrowIfFailed(rasterizer_state_.As(&render_state_));
}
