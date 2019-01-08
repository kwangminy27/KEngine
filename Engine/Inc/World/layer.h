#pragma once

namespace K
{
	class Level;

	class ENGINE_DLL Layer final : public Tag, public std::enable_shared_from_this<Layer>
	{
		friend class Level;
	public:
		virtual void Initialize() override;

		APTR const& FindActor(TAG const& _tag) const;

		void AddActor(APTR const& _actor);
		void RemoveActor(APTR const& _actor);

		std::shared_ptr<Level> level() const;
		void set_level(std::shared_ptr<Level> const& _level);

		static APTR actor_dummy_;

	private:
		Layer() = default;
		Layer(Layer const&) = delete;
		Layer(Layer&& _other) noexcept;
		Layer& operator=(Layer const&) = delete;
		Layer& operator=(Layer&&) noexcept = default;

		virtual void _Finalize() override;

		void _Input(float _time);
		void _Update(float _time);
		void _Collision(float _time);
		void _Render(float _time);

		std::weak_ptr<Level> level_{};
		std::list<APTR> actor_list_{};
	};
}
