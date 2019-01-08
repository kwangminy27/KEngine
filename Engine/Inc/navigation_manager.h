#pragma once

namespace K
{
	class TileMapActor;

	struct A_STAR_DESC
	{
		float G;
		float H;
		float F;
		std::pair<int, int> idx;
		std::shared_ptr<A_STAR_DESC> parent;
	};

	class ENGINE_DLL NavigationManager final : public Singleton<NavigationManager>
	{
		friend class Singleton<NavigationManager>;
	public:
		virtual void Initialize() override;

		bool Route(Vector3 const& _start, Vector3 const& _end);

		void AddTileMap(std::shared_ptr<TileMapActor> const& _tile_map);

		std::list<Vector3> const& move_path_list() const;

	private:
		NavigationManager() = default;
		NavigationManager(NavigationManager const&) = delete;
		NavigationManager(NavigationManager&&) noexcept = delete;
		NavigationManager& operator=(NavigationManager const&) = delete;
		NavigationManager& operator=(NavigationManager&&) noexcept = delete;

		virtual void _Finalize() override;

		std::list<Vector3> move_path_list_{};
		std::list<std::shared_ptr<TileMapActor>> tile_map_list_{};
	};
}
