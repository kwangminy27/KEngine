#include "Engine.h"
#include "blend_state.h"

#include "device_manager.h"

void K::BlendState::SetToShader()
{
	UINT sample_mask{ 0xffffffff };
	DeviceManager::singleton()->context()->OMSetBlendState(static_cast<ID3D11BlendState*>(render_state_.Get()), nullptr, sample_mask);
}

K::BlendState::BlendState(BlendState const& _other) : RenderState(_other)
{
}

K::BlendState::BlendState(BlendState&& _other) noexcept : RenderState(std::move(_other))
{
}

void K::BlendState::_CreateState(
	bool _alpha_to_coverage_enable,
	bool _independent_blend_enable,
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector)
{
	auto const& device = DeviceManager::singleton()->device();

	D3D11_BLEND_DESC dbd{};
	dbd.AlphaToCoverageEnable = _alpha_to_coverage_enable;
	dbd.IndependentBlendEnable = _independent_blend_enable;

	auto size = sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * _render_target_blend_desc_vector.size();

	memcpy_s(dbd.RenderTarget, size, _render_target_blend_desc_vector.data(), size);

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state{};
	ThrowIfFailed(device->CreateBlendState(&dbd, &blend_state));

	ThrowIfFailed(blend_state.As(&render_state_));
}
