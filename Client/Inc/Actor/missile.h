#pragma once

namespace K
{
	class Missile final : public ActorClient
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Missile() = default;
		Missile(Missile const& _other);
		Missile(Missile&& _other) noexcept;
		Missile& operator=(Missile const&) = delete;
		Missile& operator=(Missile&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;
	};
}
