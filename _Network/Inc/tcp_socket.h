#pragma once

namespace K
{
	class Address;

	class NETWORK_DLL TCPSocket
	{
		friend class SocketManager;
	public:
		void Bind(Address const& _address);

		void Listen(int _backlog);
		std::shared_ptr<TCPSocket> Accept(Address& _from);

		void Connect(Address const& _to);

		int Send(void const* _buffer, int _size);
		int Receive(void* _buffer, int _size);

		void ShutDown();

		void SetNonBlockingMode(bool _flag);

		SOCKET const& socket() const;

	private:
		TCPSocket(SOCKET _socket);
		TCPSocket(TCPSocket&& _other) noexcept;
		TCPSocket& operator=(TCPSocket&&) noexcept = default;

		void _Finalize();

		SOCKET socket_{};
	};
}
