#include "Network.h"
#include "socket_manager.h"

#include "udp_socket.h"
#include "tcp_socket.h"

void K::SocketManager::Initialize()
{
	try
	{
		WSADATA data{};
		WSAStartup(MAKEWORD(2, 2), &data);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "SocketManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::UDPSocket> K::SocketManager::CreateUDPSocket()
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == s)
		throw std::exception{ "SocketManager::CreateUDPSocket" };

	return std::shared_ptr<UDPSocket>{ new UDPSocket{ s }, [](UDPSocket* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

std::shared_ptr<K::TCPSocket> K::SocketManager::CreateTCPSocket()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == s)
		throw std::exception{ "SocketManager::CreateTCPSocket" };

	return std::shared_ptr<TCPSocket>{ new TCPSocket{ s }, [](TCPSocket* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

std::shared_ptr<K::TCPSocket> K::SocketManager::CreateOverlappedTCPSocket()
{
	SOCKET s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == s)
		throw std::exception{ "SocketManager::CreateOverlappedTCPSocket" };

	return std::shared_ptr<TCPSocket>{ new TCPSocket{ s }, [](TCPSocket* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::SocketManager::_Finalize()
{
	WSACleanup();
}
