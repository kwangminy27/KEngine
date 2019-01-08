#pragma once

namespace K
{
	class IntroLevel final : public Level
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

	private:
		IntroLevel() = default;
		IntroLevel(IntroLevel const&) = delete;
		IntroLevel(IntroLevel&& _other) noexcept;
		IntroLevel& operator=(IntroLevel const&) = delete;
		IntroLevel& operator=(IntroLevel&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time);
		virtual void _Update(float _time);
		virtual void _Collision(float _time);
		virtual void _Render(float _time);
	};
}
