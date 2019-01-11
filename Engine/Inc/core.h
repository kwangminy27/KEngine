#pragma once

namespace K
{
	class ENGINE_DLL Core final : public Singleton<Core>
	{
		friend class Singleton<Core>;
	public:
		virtual void Initialize() override;

		void Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance, GAME_MODE _mode);
		void Initialize(HINSTANCE _instance, HWND _window, GAME_MODE _mode);

		void Run();
		void Logic();

	private:
		Core() = default;
		Core(Core const&) = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(Core const&) = delete;
		Core& operator=(Core&&) noexcept = delete;

		virtual void _Finalize() override;

		static LRESULT CALLBACK _WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param);
		void _RegisterClass(std::wstring const& _class_name);
		void _CreateWindow(std::wstring const& _class_name, std::wstring const& _window_name);

		void _Input(float _time);
		void _Update(float _time);
		void _Collision(float _time);
		void _Render(float _time);

		static bool shutdown_;
		static bool gui_show_;

		HWND window_{};
		HINSTANCE instance_{};
	};
}
