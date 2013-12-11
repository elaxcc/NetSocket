#ifndef NETMANAGER_H_
#define NETMANAGER_H_

#include "NetCommon.h"
#include "SimpleServer.h"
#include "SimpleClient.h"
#include "Parser.h"

#include "list"
#include "map"
#include "vector"

#include <poll.h>

namespace Net
{

class net_manager
{
public:
	net_manager();
	~net_manager();

	int add_server(int port, bool nonblocking, bool no_nagle_delay);
	int add_server_connection(int socket);
	int add_client(const std::string& address, int port,
		bool nonblocking, bool no_nagle_delay);

	void remove_server(int socket);
	void remove_server_connection(int socket);
	void remove_client(int socket);

	void process_sockets();
	int poll_sockets(short int poll_events, int timeout);

private:
	int process_server(int socket);
	int process_server_connections(int socket);
	int process_client(int socket);

	void remove_from_polling_list(int socket);
	int process_socket_internal(int socket);

private:
	enum member_type
	{
		member_type_server,
		member_type_server_connection,
		member_type_client
	};

	struct client_info
	{
		simple_client client_;
		parser pars_;

		client_info(const std::string& address, int port,
			bool nonblocking, bool no_nagle_delay)
			: client_(nonblocking, no_nagle_delay)
		{
		}
	};

	std::map<int, member_type> members_type_list_;
	std::map<int, simple_server> servers_list_;
	std::map<int, parser> servers_connections_list_;
	std::map<int, client_info> clients_list_;
	std::vector<pollfd> polling_list_;
};

} // name

#endif /* NETMANAGER_H_ */
