#pragma once

#include "component.h"

namespace K
{
	class TileMapActor;

	class ENGINE_DLL Navigator final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Update(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void Route(Vector3 const& _start, Vector3 const& _end);

		void set_speed(float _speed);

	private:
		Navigator() = default;
		Navigator(Navigator const& _other);
		Navigator(Navigator&& _other) noexcept;
		Navigator& operator=(Navigator const&) = delete;
		Navigator& operator=(Navigator&&) noexcept = default;

		virtual void _Finalize() override;

		float speed_{};
		bool move_flag_{};
		Vector3 destination_{};
		std::list<Vector3> move_path_list_{};
	};
}
