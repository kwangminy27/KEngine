#pragma once

// Windows API
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// C++ Standard Library
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

// DirectX
#include <wrl/client.h>
#include <dxgidebug.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dinput.h>

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dinput8")

// DirectXMath
#include <DirectXColors.h>

// DirectXTK
#include <Audio.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

// DirectShow
#include <dshow.h>

#pragma comment(lib, "strmiids")

// Common
#include <Common.h>
#include <tag.h>
#include <singleton.h>

// Network
#include <Network.h>
#include <address.h>
#include <socket_manager.h>
#include <udp_socket.h>
#include <tcp_socket.h>
#include <memory_stream.h>

// Physics
#include <Physics.h>
#include <type.h>

// Utility
#include <Utility.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

// Base
#include "Base/enum.h"
#include "Base/macro.h"
#include "Base/term.h"

namespace K
{
	static void ThrowIfFailed(HRESULT _result)
	{
		if (FAILED(_result))
			throw std::exception{};
	}
}
