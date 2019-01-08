#include "Engine.h"
#include "connection_manager.h"

std::unique_ptr<K::Connection> K::ConnectionManager::connection_dummy_{};

void K::ConnectionManager::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ConnectionManager::Initialize" << std::endl;
	}
}

std::unique_ptr<K::Connection> const& K::ConnectionManager::FindConnection(int _id) const
{
	auto iter = std::find_if(connection_list_.begin(), connection_list_.end(), [&_id](std::unique_ptr<Connection> const& _p) {
		return _p->id == _id;
	});

	if (iter == connection_list_.end())
		return connection_dummy_;

	return *iter;
}

void K::ConnectionManager::AddConnection(std::unique_ptr<Connection>& _connection)
{
	connection_list_.push_back(std::move(_connection));
}

void K::ConnectionManager::RemoveConnection(std::unique_ptr<Connection> const& _connection)
{
	connection_list_.remove(_connection);
}

void K::ConnectionManager::_Finalize()
{
}
