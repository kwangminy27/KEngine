#pragma once

#include "actor.h"

namespace K
{
	class ENGINE_DLL TileActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		Vector2 const& LT() const;
		Vector2 const& RB() const;
		TILE_OPTION option() const;

		void set_LT(Vector2 const& _LT);
		void set_RB(Vector2 const& _RB);
		void set_option(TILE_OPTION _option);

	private:
		TileActor() = default;
		TileActor(TileActor const& _other);
		TileActor(TileActor&& _other) noexcept;
		TileActor& operator=(TileActor const&) = delete;
		TileActor& operator=(TileActor&&) noexcept = default;

		virtual void _Finalize() override;

		Vector2 LT_;
		Vector2 RB_;
		TILE_OPTION option_{};
	};
}
