#include "Engine.h"
#include "core.h"

#include "device_manager.h"
#include "text_manager.h"
#include "path_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "audio_manager.h"
#include "video_manager.h"
#include "time_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "collision_manager.h"
#include "navigation_manager.h"
#include "registry_manager.h"
#include "replication_manager.h"
#include "connection_manager.h"
#include "gui_manager.h"
#include "fbx_loader.h"

bool K::Core::shutdown_{};
bool K::Core::gui_show_{ true };

void K::Core::Initialize()
{
}

void K::Core::Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance, GAME_MODE _mode)
{
	try
	{
		Initialize();

		_RegisterClass(_class_name);
		_CreateWindow(_class_name, _window_name);

		Initialize(_instance, window_, _mode);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::Initialize(HINSTANCE _instance, HWND _window, GAME_MODE _mode)
{
	try
	{
		ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

		instance_ = _instance;
		window_ = _window;

		SocketManager::singleton()->Initialize();

		auto const& device_manager = DeviceManager::singleton();
		device_manager->Initialize(window_);

		TextManager::singleton()->Initialize();
		PathManager::singleton()->Initialize();
		AudioManager::singleton()->Initialize();
		VideoManager::singleton()->Initialize();
		ResourceManager::singleton()->Initialize();
		
		auto const& rendering_manager = RenderingManager::singleton();
		rendering_manager->Initialize();
		rendering_manager->set_mode(_mode);

		TimeManager::singleton()->Initialize();
		InputManager::singleton()->Initialize(instance_, window_);
		WorldManager::singleton()->Initialize();
		ObjectManager::singleton()->Initialize();
		CollisionManager::singleton()->Initialize();
		NavigationManager::singleton()->Initialize();
		RegistryManager::singleton()->Initialize();
		ReplicationManager::singleton()->Initialize();
		ConnectionManager::singleton()->Initialize();

		GUIManager::singleton()->Initialize(window_, device_manager->device().Get(), device_manager->context().Get());

		FBXLoader::singleton()->Initialize();
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::Run()
{
	MSG message{};
	while (!shutdown_)
	{
		if (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
			Logic();
	}
}

void K::Core::Logic()
{
	auto const& time_manager = TimeManager::singleton();
	auto const& input_manager = InputManager::singleton();
	auto const& device_manager = DeviceManager::singleton();

	time_manager->Update();
	
	float time_delta = time_manager->time_delta();

	input_manager->Update(time_delta);

	_Input(time_delta);
	_Update(time_delta);
	_Collision(time_delta);

	device_manager->Clear();
	{
		_Render(time_delta);
	}
	device_manager->Present();
}

void K::Core::_Finalize()
{
	AudioManager::singleton()->Suspend();

	FBXLoader::singleton().reset();
	GUIManager::singleton().reset();
	ConnectionManager::singleton().reset();
	ReplicationManager::singleton().reset();
	RegistryManager::singleton().reset();
	NavigationManager::singleton().reset();
	CollisionManager::singleton().reset();
	ObjectManager::singleton().reset();
	WorldManager::singleton().reset();
	InputManager::singleton().reset();
	TimeManager::singleton().reset();
	RenderingManager::singleton().reset();
	ResourceManager::singleton().reset();
	VideoManager::singleton().reset();
	AudioManager::singleton().reset();
	PathManager::singleton().reset();
	TextManager::singleton().reset();
	DeviceManager::singleton().reset();

	SocketManager::singleton().reset();

	CoUninitialize();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param);
LRESULT K::Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(_window, _message, _w_param, _l_param))
		return true;

	switch (_message)
	{
	case WM_DESTROY:
		Core::shutdown_ = true;
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_window, _message, _w_param, _l_param);
}

void K::Core::_RegisterClass(std::wstring const& _class_name)
{
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = K::Core::_WindowProc;
	wcex.hInstance = instance_;
	wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszClassName = _class_name.c_str();
	if (!RegisterClassEx(&wcex))
		throw std::exception{ "Core::_RegisterClass" };
}

void K::Core::_CreateWindow(std::wstring const& _class_name, std::wstring const& _window_name)
{
	window_ = CreateWindowEx(
		NULL,
		_class_name.c_str(),
		_window_name.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		instance_,
		nullptr
	);

	if (!window_)
		throw std::exception{ "Core::_CreateWindow" };

	RECT rc{ 0, 0, static_cast<int>(RESOLUTION::WIDTH) , static_cast<int>(RESOLUTION::HEIGHT) };
	AdjustWindowRectEx(&rc, WS_CAPTION | WS_SYSMENU, false, NULL);

	MoveWindow(window_, 152, 70, rc.right - rc.left, rc.bottom - rc.top, true);
	ShowWindow(window_, SW_SHOW);
}

void K::Core::_Input(float _time)
{
	auto const& input_manager = InputManager::singleton();

	if (input_manager->KeyDown("ESC"))
		DestroyWindow(window_);

	if (input_manager->KeyDown("F1"))
		gui_show_ ^= true;

	WorldManager::singleton()->Input(_time);
}

void K::Core::_Update(float _time)
{
	WorldManager::singleton()->Update(_time);
}

void K::Core::_Collision(float _time)
{
	WorldManager::singleton()->Collision(_time);
}

void K::Core::_Render(float _time)
{
	WorldManager::singleton()->Render(_time);
	RenderingManager::singleton()->Render(_time);

	if (gui_show_)
		GUIManager::singleton()->Render();
}
