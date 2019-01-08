#pragma once

namespace K
{
	class UDPSocket;
	class TCPSocket;

	class NETWORK_DLL SocketManager final : public Singleton<SocketManager>
	{
		friend class Singleton<SocketManager>;
	public:
		virtual void Initialize() override;

		std::shared_ptr<UDPSocket> CreateUDPSocket();
		std::shared_ptr<TCPSocket> CreateTCPSocket();
		std::shared_ptr<TCPSocket> CreateOverlappedTCPSocket();

	private:
		SocketManager() = default;
		SocketManager(SocketManager const&) = delete;
		SocketManager(SocketManager&&) noexcept = delete;
		SocketManager& operator=(SocketManager const&) = delete;
		SocketManager& operator=(SocketManager&&) noexcept = delete;

		virtual void _Finalize() override;
	};
}
