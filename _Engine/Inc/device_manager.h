#pragma once

namespace K
{
	class ENGINE_DLL DeviceManager final : public Singleton<DeviceManager>
	{
		friend class Singleton<DeviceManager>;
	public:
		virtual void Initialize() override;

		void Initialize(HWND _window);

		void Clear();
		void Present();

		void ReportLiveObjects();

		Microsoft::WRL::ComPtr<ID3D11Device> const& device() const;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> const& context() const;

		Microsoft::WRL::ComPtr<ID2D1RenderTarget> const& d2d_render_target() const;

	private:
		DeviceManager() = default;
		DeviceManager(DeviceManager const&) = delete;
		DeviceManager(DeviceManager&&) noexcept = delete;
		DeviceManager& operator=(DeviceManager const&) = delete;
		DeviceManager& operator=(DeviceManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _InitializeDirect3D(HWND _window);
		void _InitializeDirect2D();

		HMODULE debug_dll_{};
		Microsoft::WRL::ComPtr<IDXGIDebug> debug_{};
		Microsoft::WRL::ComPtr<ID3D11Device> device_{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_{};
		Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV_{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV_{};

		Microsoft::WRL::ComPtr<ID2D1Factory> d2d_factory_{};
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> d2d_render_target_{};
	};
}
