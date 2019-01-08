#include "Engine.h"
#include "device_manager.h"

void K::DeviceManager::Initialize()
{
}

void K::DeviceManager::Initialize(HWND _window)
{
	try
	{
		_InitializeDirect3D(_window);
		_InitializeDirect2D();
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DeviceManager::Initialize" << std::endl;
	}
}

void K::DeviceManager::Clear()
{
	context_->ClearRenderTargetView(RTV_.Get(), DirectX::Colors::MediumSlateBlue);
	context_->ClearDepthStencilView(DSV_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void K::DeviceManager::Present()
{
	swap_chain_->Present(0, NULL);

	context_->OMSetRenderTargets(0, nullptr, nullptr);
	context_->OMSetRenderTargets(1, RTV_.GetAddressOf(), DSV_.Get());
}

void K::DeviceManager::ReportLiveObjects()
{
	ThrowIfFailed(debug_->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL));
}

Microsoft::WRL::ComPtr<ID3D11Device> const& K::DeviceManager::device() const
{
	return device_;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> const& K::DeviceManager::context() const
{
	return context_;
}

Microsoft::WRL::ComPtr<ID2D1RenderTarget> const& K::DeviceManager::d2d_render_target() const
{
	return d2d_render_target_;
}

void K::DeviceManager::_Finalize()
{
#ifdef _DEBUG
	ReportLiveObjects();
	FreeLibrary(debug_dll_);
#endif
}

void K::DeviceManager::_InitializeDirect3D(HWND _window)
{
	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;

	debug_dll_ = LoadLibrary(L"DXGIDebug.dll");
	ThrowIfFailed(reinterpret_cast<HRESULT(*)(REFIID, void**)>(GetProcAddress(debug_dll_, "DXGIGetDebugInterface"))(__uuidof(IDXGIDebug), &debug_));
#endif

	std::array<D3D_FEATURE_LEVEL, 1> feature_levels{ D3D_FEATURE_LEVEL_11_0 };

	ThrowIfFailed(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		feature_levels.data(),
		static_cast<UINT>(feature_levels.size()),
		D3D11_SDK_VERSION,
		&device_,
		nullptr,
		&context_
	));

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device{};
	ThrowIfFailed(device_.As(&dxgi_device));

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgi_adapter{};
	ThrowIfFailed(dxgi_device->GetAdapter(&dxgi_adapter));

	Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory{};
	ThrowIfFailed(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), &dxgi_factory));

	DXGI_SWAP_CHAIN_DESC dscd{};
	dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dscd.SampleDesc.Count = 1;
	dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dscd.BufferCount = 2;
	dscd.OutputWindow = _window;
	dscd.Windowed = true;
	dscd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	ThrowIfFailed(dxgi_factory->CreateSwapChain(device_.Get(), &dscd, &swap_chain_));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer{};
	ThrowIfFailed(swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &buffer));
	ThrowIfFailed(device_->CreateRenderTargetView(buffer.Get(), nullptr, &RTV_));

	D3D11_TEXTURE2D_DESC dtd{};
	dtd.Width = static_cast<UINT>(RESOLUTION::WIDTH);
	dtd.Height = static_cast<UINT>(RESOLUTION::HEIGHT);
	dtd.MipLevels = 1;
	dtd.ArraySize = 1;
	dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtd.SampleDesc.Count = 1;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowIfFailed(device_->CreateTexture2D(&dtd, nullptr, &buffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC ddsvd{};
	ddsvd.Format = dtd.Format;
	ddsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ThrowIfFailed(device_->CreateDepthStencilView(buffer.Get(), &ddsvd, &DSV_));

	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<int>(RESOLUTION::WIDTH);
	viewport.Height = static_cast<int>(RESOLUTION::HEIGHT);
	viewport.MaxDepth = 1.f;

	context_->RSSetViewports(1, &viewport);
	context_->OMSetRenderTargets(1, RTV_.GetAddressOf(), DSV_.Get());
}

void K::DeviceManager::_InitializeDirect2D()
{
#ifdef _DEBUG
	D2D1_FACTORY_OPTIONS dfo{ D2D1_DEBUG_LEVEL_INFORMATION };
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &dfo, &d2d_factory_));
#else
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &d2d_factory_));
#endif

	Microsoft::WRL::ComPtr<IDXGISurface> surface{};
	ThrowIfFailed(swap_chain_->GetBuffer(0, __uuidof(IDXGISurface), &surface));

	D2D1_RENDER_TARGET_PROPERTIES drtp{};
	drtp.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	drtp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	drtp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

	d2d_factory_->CreateDxgiSurfaceRenderTarget(surface.Get(), &drtp, &d2d_render_target_);
}
