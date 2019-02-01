#pragma once

#include "component.h"

namespace K
{
	enum class PARENT_FLAG
	{
		SCALING = 1 << 0,
		ROTATION = 1 << 1,
		TRANSLATION = 1 << 2
	};

	class ENGINE_DLL Transform final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Update(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void UpdateConstantBuffer();

		bool dirty_flag() const;
		uint8_t parent_flag() const;

		Vector3 const& model_scaling() const;
		Vector3 const& local_scaling() const;
		Vector3 const& parent_scaling() const;
		Vector3 const& world_scaling() const;

		Quaternion const& model_rotation() const;
		Quaternion const& local_rotation() const;
		Quaternion const& parent_rotation() const;
		Quaternion const& world_rotation() const;

		Vector3 const& model_translation() const;
		Vector3 const& local_translation() const;
		Vector3 const& parent_translation() const;
		Vector3 const& world_translation() const;

		Matrix const& model() const;
		Matrix const& local() const;
		Matrix const& parent() const;
		Matrix const& world() const;

		void set_dirty_flag(bool _flag);
		void set_parent_flag(uint8_t _flag);

		void set_model_scaling(Vector3 const& _v);
		void set_local_scaling(Vector3 const& _v);
		void set_parent_scaling(Vector3 const& _v);

		void set_model_rotation(Quaternion const& _q);
		void set_local_rotation(Quaternion const& _q);
		void set_parent_rotation(Quaternion const& _q);

		void set_model_translation(Vector3 const& _v);
		void set_local_translation(Vector3 const& _v);
		void set_parent_translation(Vector3 const& _v);

	private:
		Transform() = default;
		Transform(Transform const& _other);
		Transform(Transform&& _other) noexcept;
		Transform& operator=(Transform const&) = delete;
		Transform& operator=(Transform&&) noexcept = default;

		virtual void _Finalize() override;

		bool dirty_flag_{ true };
		uint8_t parent_flag_{};

		Vector3 model_scaling_{ Vector3::One };
		Vector3 local_scaling_{ Vector3::One };
		Vector3 parent_scaling_{ Vector3::One };
		Vector3 world_scaling_{ Vector3::One };

		Quaternion model_rotation_{ Quaternion::Identity };
		Quaternion local_rotation_{ Quaternion::Identity };
		Quaternion parent_rotation_{ Quaternion::Identity };
		Quaternion world_rotation_{ Quaternion::Identity };

		Vector3 model_translation_{};
		Vector3 local_translation_{};
		Vector3 parent_translation_{};
		Vector3 world_translation_{};

		Matrix model_{ Matrix::Identity };
		Matrix local_{ Matrix::Identity };
		Matrix parent_{ Matrix::Identity };
		Matrix world_{ Matrix::Identity };
	};
}
