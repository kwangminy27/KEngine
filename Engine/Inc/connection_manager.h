#pragma once

namespace K
{
	struct Connection
	{
		int id;
		std::shared_ptr<TCPSocket> socket;
		InputMemoryStream imstream;
		OutputMemoryStream omstream;
	};

	class ENGINE_DLL ConnectionManager : public Singleton<ConnectionManager>
	{
		friend class Singleton<ConnectionManager>;
	public:
		virtual void Initialize() override;

		std::unique_ptr<Connection> const& FindConnection(int _id) const;

		void AddConnection(std::unique_ptr<Connection>& _connection);
		void RemoveConnection(std::unique_ptr<Connection> const& _connection);

		static std::unique_ptr<Connection> connection_dummy_;

	private:
		ConnectionManager() = default;
		ConnectionManager(ConnectionManager const&) = delete;
		ConnectionManager(ConnectionManager&&) noexcept = delete;
		ConnectionManager& operator=(ConnectionManager const&) = delete;
		ConnectionManager& operator=(ConnectionManager&&) noexcept = delete;

		virtual void _Finalize() override;

		std::list<std::unique_ptr<Connection>> connection_list_{};
	};
}
