#pragma once

#include "collider.h"

namespace K
{
	class ENGINE_DLL ColliderAABB final : public Collider
	{
		friend class ObjectManager;
		friend class CollisionManager;
	public:
		virtual void Initialize() override;

		virtual void Update(float _time) override;
		virtual void Render(float _time) override;

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		AABB const& absolute_info() const;

		void set_relative_info(AABB const& _info);

	private:
		ColliderAABB() = default;
		ColliderAABB(ColliderAABB const& _other);
		ColliderAABB(ColliderAABB&& _other) noexcept;
		ColliderAABB& operator=(ColliderAABB const&) = delete;
		ColliderAABB& operator=(ColliderAABB&&) noexcept = default;

		virtual bool _Collision(Collider* _dest, float _time) override;

		AABB relative_info_{};
		AABB absolute_info_{};
	};
}
