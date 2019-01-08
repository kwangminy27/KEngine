#include "Network.h"
#include "address.h"

K::Address::Address(uint32_t _ip_address, uint16_t _port)
{
	reinterpret_cast<sockaddr_in*>(&sockaddr_)->sin_family = AF_INET;
	reinterpret_cast<sockaddr_in*>(&sockaddr_)->sin_addr.S_un.S_addr = htonl(_ip_address);
	reinterpret_cast<sockaddr_in*>(&sockaddr_)->sin_port = htons(_port);
}

K::Address::Address(sockaddr const& _sockaddr)
{
	memcpy_s(&sockaddr_, GetSize(), &_sockaddr, GetSize());
}

int K::Address::GetSize() const
{
	return static_cast<int>(sizeof(sockaddr_));
}
