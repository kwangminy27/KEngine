#pragma once

namespace K
{
	class ENGINE_DLL GUIManager final : public Singleton<GUIManager>
	{
		friend class Singleton<GUIManager>;
	public:
		virtual void Initialize() override;

		void Initialize(HWND _window, ID3D11Device* _device, ID3D11DeviceContext* _context);

		void Render(std::string const& _title_name);

	private:
		GUIManager() = default;
		GUIManager(GUIManager const&) = delete;
		GUIManager(GUIManager&&) noexcept = delete;
		GUIManager& operator=(GUIManager const&) = delete;
		GUIManager& operator=(GUIManager&&) noexcept = delete;

		virtual void _Finalize() override;
	};
}
