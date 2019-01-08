#pragma once

#include "collider.h"

namespace K
{
	class ENGINE_DLL ColliderPoint final : public Collider
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

		Vector3 const& absolute_info() const;

		void set_relative_info(Vector3 const& _info);

	private:
		ColliderPoint() = default;
		ColliderPoint(ColliderPoint const& _other);
		ColliderPoint(ColliderPoint&& _other) noexcept;
		ColliderPoint& operator=(ColliderPoint const&) = delete;
		ColliderPoint& operator=(ColliderPoint&&) noexcept = default;

		virtual bool _Collision(Collider* _dest, float _time) override;

		Vector3 relative_info_{};
		Vector3 absolute_info_{};
	};
}
