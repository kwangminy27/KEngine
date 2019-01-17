#pragma once

#include "actor.h"

namespace K
{
	class ENGINE_DLL LightActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		virtual void _Render(float _time) override;

	private:
		LightActor() = default;
		LightActor(LightActor const& _other);
		LightActor(LightActor&& _other) noexcept;
		LightActor& operator=(LightActor const&) = delete;
		LightActor& operator=(LightActor&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
