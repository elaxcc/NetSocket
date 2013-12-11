#include "OperatorServer.h"

#include "NetManager.h"

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

int operator_server::process_event()
{
	int new_client_socket;
	struct sockaddr_in new_client_addr;

	int accept_result = client_accept(
		&new_client_socket, &new_client_addr);

	if (accept_result == error_no_)
	{
		//net_manager_->
	}

	return accept_result;
}

} // Net
