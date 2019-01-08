#pragma once

namespace K
{
	class Layer;

	class ENGINE_DLL Level : public Tag, public std::enable_shared_from_this<Level>
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Layer> const& FindLayer(TAG const& _tag) const;
		APTR const& FindActor(TAG const& _tag) const;

		std::shared_ptr<Layer> CreateLayer(TAG const& _tag);

		static std::shared_ptr<Layer> layer_dummy_;

	protected:
		Level() = default;
		Level(Level const&) = delete;
		Level(Level&& _other) noexcept;
		Level& operator=(Level const&) = delete;
		Level& operator=(Level&&) noexcept = default;
		virtual ~Level() = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time);
		virtual void _Update(float _time);
		virtual void _Collision(float _time);
		virtual void _Render(float _time);

		void __Input(float _time);
		void __Update(float _time);
		void __Collision(float _time);
		void __Render(float _time);

	private:
		std::list<std::shared_ptr<Layer>> layer_list_{};
	};
}
