#pragma once

namespace K
{
	class ENGINE_DLL RegistryManager final : public Singleton<RegistryManager>
	{
		friend class Singleton<RegistryManager>;
	public:
		virtual void Initialize() override;

		std::function<APTR(TAG const&)> const& FindActorGenerator(std::string const& _tag) const;

		void AddActorGenerator(std::string const& _tag, std::function<APTR(TAG const&)> const& _function);

		static std::function<APTR(TAG const&)> actor_generator_dummy_;

	private:
		RegistryManager() = default;
		RegistryManager(RegistryManager const&) = delete;
		RegistryManager(RegistryManager&&) noexcept = delete;
		RegistryManager& operator=(RegistryManager const&) = delete;
		RegistryManager& operator=(RegistryManager&&) noexcept = delete;

		virtual void _Finalize() override;

		std::unordered_map<std::string, std::function<APTR(TAG const&)>> actor_generator_map_{};
	};
}
