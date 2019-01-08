#pragma once

namespace K
{
	class ENGINE_DLL Component : public Tag
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Input(float _time);
		virtual void Update(float _time);
		virtual void Collision(float _time);
		virtual void Render(float _time);

		virtual CPTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

		APTR owner() const;

		void set_owner(APTR const& _actor);

	protected:
		Component() = default;
		Component(Component const& _other);
		Component(Component&& _other) noexcept;
		Component& operator=(Component const&) = delete;
		Component& operator=(Component&&) noexcept = default;
		virtual ~Component() = default;

		virtual void _Finalize() override;

		std::weak_ptr<Actor> owner_{};
	};
}
