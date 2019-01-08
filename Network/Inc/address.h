#pragma once

namespace K
{
	class NETWORK_DLL Address
	{
		friend class UDPSocket;
		friend class TCPSocket;
	public:
		Address() = default;
		Address(uint32_t _ip_address, uint16_t _port);
		Address(sockaddr const& _sockaddr);

		int GetSize() const;

	private:
		sockaddr sockaddr_;
	};
}
