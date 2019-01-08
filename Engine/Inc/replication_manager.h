#pragma once

namespace K
{
	class InputMemoryStream;
	class OutputMemoryStream;

	class ENGINE_DLL ReplicationManager final : public Singleton<ReplicationManager>
	{
		friend class Singleton<ReplicationManager>;
	public:
		virtual void Initialize() override;

		void SendWorld(OutputMemoryStream& _omstream);
		void ReceiveWorld(InputMemoryStream& _imstream);

		void AddActor(APTR const& _actor);
		void RemoveActor(APTR const& _actor);

	private:
		ReplicationManager() = default;
		ReplicationManager(ReplicationManager const&) = delete;
		ReplicationManager(ReplicationManager&&) noexcept = delete;
		ReplicationManager& operator=(ReplicationManager const&) = delete;
		ReplicationManager& operator=(ReplicationManager&&) noexcept = delete;

		virtual void _Finalize() override;

		std::list<APTR> replicable_actor_list_{};
	};
}
