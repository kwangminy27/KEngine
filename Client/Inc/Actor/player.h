#pragma once

namespace K
{
	class Player final : public ActorClient
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Player() = default;
		Player(Player const& _other);
		Player(Player&& _other) noexcept;
		Player& operator=(Player const&) = delete;
		Player& operator=(Player&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;
	};
}
