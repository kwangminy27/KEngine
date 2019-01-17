#include "Engine.h"
#include "mrt.h"

#include "device_manager.h"
#include "rendering_manager.h"
#include "render_target.h"

void K::MRT::AddRTV(std::string const& _tag)
{
	auto iter = RenderingManager::singleton()->FindRenderTarget(_tag);

	if (nullptr == iter)
		throw std::exception{ "MRT::AddRTV" };

	RTV_vector_.push_back(iter->RTV_);
	old_RTV_vector_.resize(RTV_vector_.size());

	SRV_vector_.push_back(iter->SRV_);
}

void K::MRT::AddDSV(std::string const& _tag)
{
	auto iter = RenderingManager::singleton()->FindRenderTarget(_tag);

	if (nullptr == iter)
		throw std::exception{ "MRT::AddDSV" };

	DSV_ = iter->DSV_;
}

void K::MRT::Clear()
{
	auto const& context = DeviceManager::singleton()->context();

	for (auto& RTV : RTV_vector_)
		context->ClearRenderTargetView(RTV.Get(), DirectX::Colors::Black);

	if (DSV_)
		context->ClearDepthStencilView(DSV_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void K::MRT::SetTarget()
{
	auto const& context = DeviceManager::singleton()->context();

	context->OMGetRenderTargets(static_cast<UINT>(old_RTV_vector_.size()), &old_RTV_vector_.at(0), &old_DSV_);

	auto DSV = old_DSV_;

	if (DSV_)
		DSV = DSV_;

	context->OMSetRenderTargets(static_cast<UINT>(RTV_vector_.size()), RTV_vector_.at(0).GetAddressOf(), DSV.Get());
}

void K::MRT::ResetTarget()
{
	DeviceManager::singleton()->context()->OMSetRenderTargets(static_cast<UINT>(old_RTV_vector_.size()), old_RTV_vector_.at(0).GetAddressOf(), old_DSV_.Get());

	old_DSV_ = nullptr;

	for (auto& old_RTV : old_RTV_vector_)
		old_RTV = nullptr;
}

void K::MRT::Attach(int _slot)
{
	DeviceManager::singleton()->context()->PSSetShaderResources(_slot, static_cast<UINT>(SRV_vector_.size()), SRV_vector_.at(0).GetAddressOf());
}

void K::MRT::Detach(int _slot)
{
	static std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> nullptr_SRV_vector{};
	nullptr_SRV_vector.resize(SRV_vector_.size());
	DeviceManager::singleton()->context()->PSSetShaderResources(_slot, static_cast<UINT>(nullptr_SRV_vector.size()), nullptr_SRV_vector.at(0).GetAddressOf());
}

K::MRT::MRT(MRT const& _other)
{
	DSV_ = _other.DSV_;
	old_DSV_ = _other.old_DSV_;
	RTV_vector_ = _other.RTV_vector_;
	old_RTV_vector_ = _other.old_RTV_vector_;
}

K::MRT::MRT(MRT&& _other) noexcept
{
	DSV_ = std::move(_other.DSV_);
	old_DSV_ = std::move(_other.old_DSV_);
	RTV_vector_ = std::move(_other.RTV_vector_);
	old_RTV_vector_ = std::move(_other.old_RTV_vector_);
}
