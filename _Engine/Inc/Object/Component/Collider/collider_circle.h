#pragma once

#include "collider.h"

namespace K
{
	class ENGINE_DLL ColliderCircle final : public Collider
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

		Circle const& absolute_info() const;

		void set_relative_info(Circle const& _info);

	private:
		ColliderCircle() = default;
		ColliderCircle(ColliderCircle const& _other);
		ColliderCircle(ColliderCircle&& _other) noexcept;
		ColliderCircle& operator=(ColliderCircle const&) = delete;
		ColliderCircle& operator=(ColliderCircle&&) noexcept = default;

		virtual bool _Collision(Collider* _dest, float _time) override;

		Circle relative_info_{};
		Circle absolute_info_{};
	};
}
