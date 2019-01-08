#include "Engine.h"
#include "sampler.h"

#include "device_manager.h"

void K::Sampler::SetToShader(int _slot)
{
	DeviceManager::singleton()->context()->PSSetSamplers(_slot, 1, sampler_.GetAddressOf());
}

K::Sampler::Sampler(Sampler const& _other)
{
	sampler_ = _other.sampler_;
}

K::Sampler::Sampler(Sampler&& _other) noexcept
{
	sampler_ = std::move(_other.sampler_);
}

void K::Sampler::_CreateSampler(
	D3D11_FILTER _filter,
	D3D11_TEXTURE_ADDRESS_MODE _address_u,
	D3D11_TEXTURE_ADDRESS_MODE _address_v,
	D3D11_TEXTURE_ADDRESS_MODE _address_w)
{
	auto const& device = DeviceManager::singleton()->device();

	D3D11_SAMPLER_DESC dsd{};
	dsd.Filter = _filter;
	dsd.AddressU = _address_u;
	dsd.AddressV = _address_v;
	dsd.AddressW = _address_w;
	dsd.MaxAnisotropy = 1;

	ThrowIfFailed(device->CreateSamplerState(&dsd, &sampler_));
}
