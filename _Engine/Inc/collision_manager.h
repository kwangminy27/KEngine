#pragma once

namespace K
{
	class Collider;

	using CollisionSection = std::vector<Collider*>;

	struct CollisionGroup
	{
		Vector3 min;
		Vector3 max;
		int x_count;
		int y_count;
		int z_count;
		int total_count;
		Vector3 section_diagonal_length;
		std::unique_ptr<CollisionSection[], std::function<void(CollisionSection*)>> section;
	};

	class ENGINE_DLL CollisionManager final : public Singleton<CollisionManager>
	{
		friend class Singleton<CollisionManager>;
	public:
		virtual void Initialize() override;

		void Collision(float _time);

		std::unique_ptr<CollisionGroup> const& FindCollisionGroup(std::string const& _tag) const;

		void AddCollider(APTR const& _actor);

		static std::unique_ptr<CollisionGroup> collision_group_dummy_;

	private:
		CollisionManager() = default;
		CollisionManager(CollisionManager const&) = delete;
		CollisionManager(CollisionManager&&) noexcept = delete;
		CollisionManager& operator=(CollisionManager const&) = delete;
		CollisionManager& operator=(CollisionManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateCollisionGroup(std::string const& _tag, Vector3 const& _min, Vector3 const& _max, int _x_count, int _y_count, int _z_count);

		std::unordered_map<std::string, std::unique_ptr<CollisionGroup>> collision_group_map_{};
	};
}
