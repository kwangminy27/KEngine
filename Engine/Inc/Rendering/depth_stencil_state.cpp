#include "Engine.h"
#include "depth_stencil_state.h"

#include "device_manager.h"

void K::DepthStencilState::SetState()
{
	auto const& context = DeviceManager::singleton()->context();

	context->OMGetDepthStencilState(reinterpret_cast<ID3D11DepthStencilState**>(old_render_state_.ReleaseAndGetAddressOf()), &old_stencil_ref_);
	context->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(render_state_.Get()), stencil_ref_);
}

void K::DepthStencilState::ResetState()
{
	DeviceManager::singleton()->context()->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(old_render_state_.Get()), old_stencil_ref_);
}

K::DepthStencilState::DepthStencilState(DepthStencilState const& _other) : RenderState(_other)
{
}

K::DepthStencilState::DepthStencilState(DepthStencilState&& _other) noexcept : RenderState(std::move(_other))
{
}

void K::DepthStencilState::_CreateState(
	BOOL _depth_enable,
	D3D11_DEPTH_WRITE_MASK _depth_write_mask,
	D3D11_COMPARISON_FUNC _depth_func,
	BOOL _stencil_enable,
	UINT8 _stencil_read_mask,
	UINT8 _stencil_write_mask,
	D3D11_DEPTH_STENCILOP_DESC const& _front_face,
	D3D11_DEPTH_STENCILOP_DESC const& _back_face)
{
	auto const& device = DeviceManager::singleton()->device();

	D3D11_DEPTH_STENCIL_DESC dsd{};
	dsd.DepthEnable = _depth_enable;
	dsd.DepthWriteMask = _depth_write_mask;
	dsd.DepthFunc = _depth_func;
	dsd.StencilEnable = _stencil_enable;
	dsd.StencilReadMask = _stencil_read_mask;
	dsd.StencilWriteMask = _stencil_write_mask;
	dsd.FrontFace = _front_face;
	dsd.BackFace = _back_face;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state{};
	ThrowIfFailed(device->CreateDepthStencilState(&dsd, &depth_stencil_state));

	ThrowIfFailed(depth_stencil_state.As(&render_state_));
}
