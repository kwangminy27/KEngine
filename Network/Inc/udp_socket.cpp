#include "Network.h"
#include "udp_socket.h"

#include "address.h"

void K::UDPSocket::Bind(Address const& _address)
{
	if (SOCKET_ERROR == bind(socket_, &_address.sockaddr_, _address.GetSize()))
		throw std::exception{ "UDPSocket::Bind" };
}

int K::UDPSocket::SendTo(void const* _buffer, int _size, Address const& _to)
{
	auto result = sendto(socket_, static_cast<char const*>(_buffer), _size, NULL, &_to.sockaddr_, _to.GetSize());

	if (SOCKET_ERROR == result)
		throw std::exception{ "UDPSocket::SendTo" };

	return result;
}

int K::UDPSocket::ReceiveFrom(void* _buffer, int _size, Address& _from)
{
	int size = _from.GetSize();

	auto result = recvfrom(socket_, static_cast<char*>(_buffer), _size, NULL, &_from.sockaddr_, &size);

	if (SOCKET_ERROR == result)
		throw std::exception{ "UDPSocket::ReceiveFrom" };

	return result;
}

void K::UDPSocket::ShutDown()
{
	if (SOCKET_ERROR == shutdown(socket_, SD_BOTH))
		std::exception{ "UDPSocket::ShutDown" };
}

SOCKET const& K::UDPSocket::socket() const
{
	return socket_;
}

K::UDPSocket::UDPSocket(SOCKET _socket)
{
	socket_ = _socket;
}

K::UDPSocket::UDPSocket(UDPSocket&& _other) noexcept
{
	*this = std::move(_other);
}

void K::UDPSocket::_Finalize()
{
	if (SOCKET_ERROR == closesocket(socket_))
		std::exception{ "UDPSocket::Finalize" };
}
