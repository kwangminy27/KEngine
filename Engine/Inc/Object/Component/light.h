#pragma once

#include "component.h"

namespace K
{
	class ENGINE_DLL Light final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Input(float _time);
		virtual void Update(float _time);
		virtual void Collision(float _time);
		virtual void Render(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Light() = default;
		Light(Light const& _other);
		Light(Light&& _other) noexcept;
		Light& operator=(Light const&) = delete;
		Light& operator=(Light&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
