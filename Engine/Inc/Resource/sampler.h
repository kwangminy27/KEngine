#pragma once

namespace K
{
	class ENGINE_DLL Sampler
	{
		friend class ResourceManager;
	public:
		void SetToShader(int _slot);

	private:
		Sampler() = default;
		Sampler(Sampler const& _other);
		Sampler(Sampler&& _other) noexcept;
		Sampler& operator=(Sampler const&) = default;
		Sampler& operator=(Sampler&&) noexcept = default;

		void _CreateSampler(
			D3D11_FILTER _filter,
			D3D11_TEXTURE_ADDRESS_MODE _address_u,
			D3D11_TEXTURE_ADDRESS_MODE _address_v,
			D3D11_TEXTURE_ADDRESS_MODE _address_w);

		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_{};
	};
}
