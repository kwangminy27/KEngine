#pragma once

namespace K
{
	class ENGINE_DLL TimeManager final : public Singleton<TimeManager>
	{
		friend class Singleton<TimeManager>;
	public:
		virtual void Initialize() override;

		void Update();

		float time_delta() const;
		uint32_t FPS() const;

	private:
		TimeManager() = default;
		TimeManager(TimeManager const&) = delete;
		TimeManager(TimeManager&&) noexcept = delete;
		TimeManager& operator=(TimeManager const&) = delete;
		TimeManager& operator=(TimeManager&&) noexcept = delete;

		virtual void _Finalize() override;

		std::chrono::time_point<std::chrono::high_resolution_clock> last_time_point_{};
		float time_delta_{};
		uint32_t FPS_{};
	};
}
