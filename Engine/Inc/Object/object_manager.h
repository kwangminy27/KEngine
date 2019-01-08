#pragma once

namespace K
{
	class ENGINE_DLL ObjectManager final : public Singleton<ObjectManager>
	{
		friend class Singleton<ObjectManager>;
	public:
		virtual void Initialize() override;

		template <typename T> APTR CreateActor(TAG const& _tag);
		template <typename T> CPTR CreateComponent(TAG const& _tag);

	private:
		ObjectManager() = default;
		ObjectManager(ObjectManager const&) = delete;
		ObjectManager(ObjectManager&&) noexcept = delete;
		ObjectManager& operator=(ObjectManager const&) = delete;
		ObjectManager& operator=(ObjectManager&&) noexcept = delete;

		virtual void _Finalize() override;
	};
}

#include "object_manager.inl"
