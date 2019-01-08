#pragma once

#include "collider.h"

namespace K
{
	class ENGINE_DLL ColliderOOBB final : public Collider
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

		OOBB const& absolute_info() const;

		void set_relative_info(AABB const& _info);

	private:
		ColliderOOBB() = default;
		ColliderOOBB(ColliderOOBB const& _other);
		ColliderOOBB(ColliderOOBB&& _other) noexcept;
		ColliderOOBB& operator=(ColliderOOBB const&) = delete;
		ColliderOOBB& operator=(ColliderOOBB&&) noexcept = default;

		virtual bool _Collision(Collider* _dest, float _time) override;

		OOBB relative_info_{};
		OOBB absolute_info_{};
	};
}
