#pragma once

namespace K
{
	class Address;

	class NETWORK_DLL UDPSocket
	{
		friend class SocketManager;
	public:
		void Bind(Address const& _address);

		int SendTo(void const* _buffer, int _size, Address const& _to);
		int ReceiveFrom(void* _buffer, int _size, Address& _from);

		void ShutDown();

		SOCKET const& socket() const;

	private:
		UDPSocket(SOCKET _socket);
		UDPSocket(UDPSocket&& _other) noexcept;
		UDPSocket& operator=(UDPSocket&&) noexcept = default;

		void _Finalize();

		SOCKET socket_{};
	};
}
