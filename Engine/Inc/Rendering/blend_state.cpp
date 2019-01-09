#include "Engine.h"
#include "blend_state.h"

#include "device_manager.h"

void K::BlendState::SetState()
{
	auto const& context = DeviceManager::singleton()->context();

	context->OMGetBlendState(reinterpret_cast<ID3D11BlendState**>(old_render_state_.ReleaseAndGetAddressOf()), old_blend_factor_array_.data(), &old_sample_mask_);
	context->OMSetBlendState(static_cast<ID3D11BlendState*>(render_state_.Get()), blend_factor_array_.data(), sample_mask_);
}

void K::BlendState::ResetState()
{
	DeviceManager::singleton()->context()->OMSetBlendState(static_cast<ID3D11BlendState*>(old_render_state_.Get()), old_blend_factor_array_.data(), old_sample_mask_);
}

K::BlendState::BlendState(BlendState const& _other) : RenderState(_other)
{
	sample_mask_ = _other.sample_mask_;
	old_sample_mask_ = _other.old_sample_mask_;
	blend_factor_array_ = _other.blend_factor_array_;
	old_blend_factor_array_ = _other.old_blend_factor_array_;
}

K::BlendState::BlendState(BlendState&& _other) noexcept : RenderState(std::move(_other))
{
	sample_mask_ = std::move(_other.sample_mask_);
	old_sample_mask_ = std::move(_other.old_sample_mask_);
	blend_factor_array_ = std::move(_other.blend_factor_array_);
	old_blend_factor_array_ = std::move(_other.old_blend_factor_array_);
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
