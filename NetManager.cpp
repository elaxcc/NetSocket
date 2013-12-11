#include "NetManager.h"

namespace Net
{

net_manager::net_manager()
{
}

net_manager::~net_manager()
{
	members_type_list_.clear();
	servers_list_.clear();
	servers_connections_list_.clear();
	clients_list_.clear();
}

int net_manager::add_server(int port,
	bool nonblocking, bool no_nagle_delay)
{
	simple_server new_server(nonblocking, no_nagle_delay);

	int start_listen_result = new_server.start_listen(port);
	if (start_listen_result != error_no_)
	{
		return start_listen_result;
	}

	pollfd fd = {new_server.get_socket(), 0, 0};
	polling_list_.push_back(fd);

	members_type_list_.insert(
		std::pair<int, member_type>(
			new_server.get_socket(), member_type_server));
	servers_list_.insert(
		std::pair<int, simple_server>(
			new_server.get_socket(), new_server));

	return error_no_;
}

int net_manager::add_server_connection(int socket)
{
	pollfd fd = {socket, 0, 0};
	polling_list_.push_back(fd);

	members_type_list_.insert(
		std::pair<int, member_type>(
			socket, member_type_server_connection));
	servers_connections_list_.insert(
		std::pair<int, simple_server>(
			socket, parser()));

	return error_no_;
}

int net_manager::add_client(const std::string& address, int port,
	bool nonblocking, bool no_nagle_delay)
{
	simple_client new_client(nonblocking, no_nagle_delay);

	int start_listen_result = new_client.connect_to(address, port);
	if (start_listen_result != error_no_)
	{
		return start_listen_result;
	}

	pollfd fd = {new_client.get_socket(), 0, 0};
	polling_list_.push_back(fd);

	members_type_list_.insert(
		std::pair<int, member_type>(
			new_client.get_socket(), member_type_server));
	clients_list_.insert(
		std::pair<int, simple_client>(
			new_client.get_socket(), new_client));

	return error_no_;
}

void net_manager::remove_server(int socket)
{
	std::map<int, member_type>::iterator iter_type;
	iter_type = members_type_list_.find(socket);
	if (iter_type != members_type_list_.end())
	{
		members_type_list_.erase(iter_type);
	}

	std::map<int, simple_server>::iterator iter_server;
	iter_server = servers_list_.find(socket);
	if (iter_server != servers_list_.end())
	{
		servers_list_.erase(iter_server);
	}

	remove_from_polling_list(socket);
}

void net_manager::remove_server_connection(int socket)
{
	std::map<int, member_type>::iterator iter_type;
	iter_type = members_type_list_.find(socket);
	if (iter_type != members_type_list_.end())
	{
		members_type_list_.erase(iter_type);
	}

	std::map<int, parser>::iterator iter_connect;
	iter_connect = servers_connections_list_.find(socket);
	if (iter_connect != servers_connections_list_.end())
	{
		servers_connections_list_.erase(iter_connect);
	}

	remove_from_polling_list(socket);
}

void net_manager::remove_client(int socket)
{
	std::map<int, member_type>::iterator iter_type;
	iter_type = members_type_list_.find(socket);
	if (iter_type != members_type_list_.end())
	{
		members_type_list_.erase(iter_type);
	}

	std::map<int, client_info>::iterator iter_client;
	iter_client = clients_list_.find(socket);
	if (iter_client != clients_list_.end())
	{
		clients_list_.erase(iter_client);
	}

	remove_from_polling_list(socket);
}

int net_manager::process_socket_internal(int socket)
{
	std::map<int, member_type>::iterator iter;

	iter = members_type_list_.find(socket);
	if (iter != members_type_list_.end())
	{
		switch (iter->second)
		{
			case member_type_server :
			{
				return process_server(socket);
			}
			case member_type_server_connection :
			{
				return process_server_connections(socket);
			}
			case member_type_client :
			{
				return process_client(socket);
			}
			default : return error_wrong_net_member_type_;
		}
	}
	return error_can_not_find_socket_;
}

void net_manager::process_sockets()
{
	int poll_result = poll_sockets(POLLIN, -1);

	if (poll_result == error_no_)
	{
		std::vector<pollfd>::iterator iter = polling_list_.begin();
		for (; iter != polling_list_.end(); ++iter)
		{
			std::map<int, member_type>::iterator member_iter =
				members_type_list_.find(iter->fd);
			if (iter != members_type_list_.end())
			{
				int process_result =
					process_socket_internal(member_iter->first);

				// !fixme обработать ошибку process_result
			}
		}
	}
}

int net_manager::process_server(int socket)
{
	std::map<int, simple_server>::iterator iter =
		servers_list_.find(socket);
	if (iter != servers_list_.end())
	{
		int new_client_socket;
		struct sockaddr_in new_client_addr;
		int accept_result = iter->second.client_accept(
			&new_client_socket, &new_client_addr);
		if (accept_result == error_no_)
		{
			add_server_connection(new_client_socket);
		}
		return accept_result;
	}
	return error_can_not_find_server_;
}

int net_manager::process_server_connections(int socket)
{
	return 0;
}

int net_manager::process_client(int socket)
{
	return 0;
}

int net_manager::poll_sockets(short int poll_events, int timeout)
{
	std::vector<pollfd>::iterator iter = polling_list_.begin();
	for (; iter != polling_list_.end(); ++iter)
	{
		if (iter->fd == socket)
		{
			iter->events = poll_events;
			iter->revents = 0;
		}
	}

	int result = poll(polling_list_.data(),
		polling_list_.size(), -1);

	if (result == 0)
	{
		return error_poll_timeout_;
	}
	else if (result < 0)
	{
		return error_poll_;
	}
	return error_no_;
}

void net_manager::remove_from_polling_list(int socket)
{
	std::vector<pollfd>::iterator iter = polling_list_.begin();
	for (; iter != polling_list_.end(); ++iter)
	{
		if (iter->fd == socket)
		{
			polling_list_.erase(iter);
			return;
		}
	}
}

} // nemaspace Net
