#include "Network.h"
#include "tcp_socket.h"

#include "address.h"

void K::TCPSocket::Bind(Address const& _address)
{
	if (SOCKET_ERROR == bind(socket_, &_address.sockaddr_, _address.GetSize()))
		throw std::exception{ "TCPSocket::Bind" };
}

void K::TCPSocket::Listen(int _backlog)
{
	if (SOCKET_ERROR == listen(socket_, _backlog))
		throw std::exception{ "TCPSocket::Listen" };
}

std::shared_ptr<K::TCPSocket> K::TCPSocket::Accept(Address& _from)
{
	int size = _from.GetSize();

	SOCKET s = accept(socket_, &_from.sockaddr_, &size);

	if (INVALID_SOCKET == s)
		throw std::exception{ "TCPSocket::Accept" };

	return std::shared_ptr<TCPSocket>{ new TCPSocket{ s }, [](TCPSocket* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TCPSocket::Connect(Address const& _to)
{
	if (SOCKET_ERROR == connect(socket_, &_to.sockaddr_, _to.GetSize()))
		throw std::exception{ "TCPSocket::Connect" };
}

int K::TCPSocket::Send(void const* _buffer, int _size)
{
	auto result = send(socket_, static_cast<char const*>(_buffer), _size, NULL);

	if (SOCKET_ERROR == result)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			throw std::exception{ "TCPSocket::Send" };
	}

	return result;
}

int K::TCPSocket::Receive(void* _buffer, int _size)
{
	auto result = recv(socket_, static_cast<char*>(_buffer), _size, NULL);

	if (SOCKET_ERROR == result)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			throw std::exception{ "TCPSocket::Receive" };
	}

	return result;
}

void K::TCPSocket::ShutDown()
{
	if (SOCKET_ERROR == shutdown(socket_, SD_BOTH))
		throw std::exception{ "TCPSocket::ShutDown" };
}

void K::TCPSocket::SetNonBlockingMode(bool _flag)
{
	u_long arg = _flag;

	if (SOCKET_ERROR == ioctlsocket(socket_, FIONBIO, &arg))
		throw std::exception{ "TCPSocket::SetNonBlockingMode" };
}

SOCKET const& K::TCPSocket::socket() const
{
	return socket_;
}

K::TCPSocket::TCPSocket(SOCKET _socket)
{
	socket_ = _socket;
}

K::TCPSocket::TCPSocket(TCPSocket&& _other) noexcept
{
	*this = std::move(_other);
}

void K::TCPSocket::_Finalize()
{
	if (SOCKET_ERROR == closesocket(socket_))
		std::exception{ "TCPSocket::Finalize" };
}
