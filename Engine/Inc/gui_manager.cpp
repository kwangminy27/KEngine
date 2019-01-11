#include "Engine.h"
#include "gui_manager.h"

void K::GUIManager::Initialize()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
}

void K::GUIManager::Initialize(HWND _window, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	ImGui_ImplWin32_Init(_window);
	ImGui_ImplDX11_Init(_device, _context);
}

void K::GUIManager::Render(std::string const& _title_name)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin(_title_name.c_str());
	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void K::GUIManager::_Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}
