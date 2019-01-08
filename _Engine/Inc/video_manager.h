#pragma once

namespace K
{
	class ENGINE_DLL VideoManager final : public Singleton<VideoManager>
	{
		friend class Singleton<VideoManager>;
	public:
		virtual void Initialize() override;

		void Play(std::wstring const& _file_name);
		void Stop();

	private:
		VideoManager() = default;
		VideoManager(VideoManager const&) = delete;
		VideoManager(VideoManager&&) noexcept = delete;
		VideoManager& operator=(VideoManager const&) = delete;
		VideoManager& operator=(VideoManager&&) noexcept = delete;

		virtual void _Finalize() override;

		Microsoft::WRL::ComPtr<IGraphBuilder> graph_builder_{};
		Microsoft::WRL::ComPtr<IMediaControl> media_control_{};
		Microsoft::WRL::ComPtr<IVideoWindow> video_window_{};
	};
}
