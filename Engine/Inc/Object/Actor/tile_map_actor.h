#pragma once

#include "actor.h"

namespace K
{
	class TileActor;

	class ENGINE_DLL TileMapActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		bool InBounds(Vector3 const& _v);

		void CreateMap(TILE_TYPE _type, int _x_count, int _y_count, Vector2 const& _tile_size);
		void DestroyMap();

		TILE_OPTION GetTileOption(std::pair<int, int> const& _idx) const;
		Vector3 GetTilePosition(std::pair<int, int> const& _idx) const;
		std::pair<int, int> GetTileIndex(Vector3 const& _position) const;
		std::list<std::pair<int, int>> const& GetTileAdjacencyList(std::pair<int, int> const& _idx) const;

		void SetTileUV(std::pair<int, int> const& _idx, Vector2 const& _LT, Vector2 const& _RB);
		void SetTileOption(std::pair<int, int> const& _idx, TILE_OPTION _option);

	private:
		TileMapActor() = default;
		TileMapActor(TileMapActor const& _other);
		TileMapActor(TileMapActor&& _other) noexcept;
		TileMapActor& operator=(TileMapActor const&) = delete;
		TileMapActor& operator=(TileMapActor&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;

		void _CreateIsometricMap();
		void _CreateOrthographicMap();
		void _CreateGraph();

		std::pair<int, int> _GetIsometricTileIndex(Vector3 const& _position) const;
		std::pair<int, int> _GetOrthographicTileIndex(Vector3 const& _position) const;

		TILE_TYPE type_{};
		Vector2 tile_size_{};
		Vector2 tile_map_size_{};
		std::vector<std::vector<std::shared_ptr<TileActor>>> tile_map_{};
		std::vector<std::vector<std::list<std::pair<int, int>>>> tile_graph_{};
	};
}
