#pragma once

namespace K
{
	class ENGINE_DLL MRT
	{
		friend class RenderingManager;
	public:
		void Clear();

		void SetTarget();
		void ResetTarget();

		void AddRTV(std::string const& _tag);
		void AddDSV(std::string const& _tag);

	private:
		MRT() = default;
		MRT(MRT const& _other);
		MRT(MRT&& _other) noexcept;
		MRT& operator=(MRT const&) = default;
		MRT& operator=(MRT&&) noexcept = default;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV_{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> old_DSV_{};
		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> RTV_vector_{};
		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> old_RTV_vector_{};
	};
}
