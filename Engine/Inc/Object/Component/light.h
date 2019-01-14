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

		void UpdateConstantBuffer();

		Vector4 const& ambient() const;
		Vector4 const& diffuse() const;
		Vector4 const& specular() const;
		Vector3 const& position() const;
		Vector3 const& direction() const;
		Vector3 const& attenuation() const;
		float falloff() const;
		int type() const;
		float range() const;
		float in_angle() const;
		float out_angle() const;

		void set_ambient(Vector4 const& _ambient);
		void set_diffuse(Vector4 const& _diffuse);
		void set_specular(Vector4 const& _specular);
		void set_position(Vector3 const& _position);
		void set_direction(Vector3 const& _direction);
		void set_attenuation(Vector3 const& _attenuation);
		void set_falloff(float _falloff);
		void set_type(int _type);
		void set_range(float _range);
		void set_in_angle(float _in_angle);
		void set_out_angle(float _out_angle);

	private:
		Light() = default;
		Light(Light const& _other);
		Light(Light&& _other) noexcept;
		Light& operator=(Light const&) = delete;
		Light& operator=(Light&&) noexcept = default;

		virtual void _Finalize() override;

		Vector4 ambient_{};
		Vector4 diffuse_{};
		Vector4 specular_{};
		Vector3 position_{};
		Vector3 direction_{};
		Vector3 attenuation_{};
		float falloff_{};
		int type_{};
		float range_{};
		float in_angle_{};
		float out_angle_{};
	};
}
