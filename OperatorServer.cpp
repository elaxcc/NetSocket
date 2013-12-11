#include "OperatorServer.h"

#include "NetManager.h"
#include "OperatorConnection.h"

namespace Net
{

operator_server::operator_server(net_manager *net_manager,
	int port, bool nonblocking, bool no_nagle_delay)
	: simple_server(nonblocking, no_nagle_delay)
	, net_manager_(net_manager)
{
	start_listen(port);
}

operator_server::~operator_server()
{
	stop_listen();
}

int operator_server::process_events(short int polling_events)
{
	if (polling_events & POLLPRI)
	{
		// process new connection

		int new_client_socket;
		struct sockaddr_in new_client_addr;

		int accept_result = client_accept(
			&new_client_socket, &new_client_addr);

		if (accept_result == error_no_)
		{
			net_manager_->add_member(
				new operator_connection(new_client_socket));
		}

		return accept_result;
	}
	else if (polling_events & POLLIN)
	{
		// process incoming data
	}
	return error_no_;
}

int operator_server::get_socket()
{
	return simple_server::get_socket();
}

short int operator_server::get_polling_flags()
{
	return POLLIN | POLLPRI;
}

} // Net
