#include "SimpleServer.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <errno.h>
#include <string.h>

namespace
{

const int c_client_queue_size = 5;
const int c_poll_timeout = 1000;

}

namespace Net
{

simple_server::simple_server(bool nonblocking, bool no_nagle_delay)
	: socket_(-1)
	, nonblocking_(nonblocking)
	, no_nagle_delay_(no_nagle_delay)
{
}

simple_server::~simple_server()
{
	if (socket_ > 0)
	{
		shutdown(socket_, SHUT_RDWR);
		close(socket_);
	}
}

int simple_server::start_listen(int port)
{
	address_.sin_family = AF_INET;
	address_.sin_port = htons(port);
	address_.sin_addr.s_addr = htonl(INADDR_ANY);

	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ < 0)
	{
		return error_create_socket_;
	}

	int set_opt_val = 1;
	if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
			&set_opt_val, sizeof(set_opt_val)) < 0)
	{
		return error_set_opt_reuse_addr_;
	}
	if (no_nagle_delay_)
	{
		set_opt_val = 1;
		if (setsockopt(socket_, SOL_TCP, TCP_NODELAY,
				&set_opt_val, sizeof(set_opt_val)) < 0)
		{
			return error_set_opt_nodelay_;
		}
	}

	if (nonblocking_)
	{
		if (fcntl(socket_, F_SETFL, O_NONBLOCK) < 0)
		{
			return error_create_nonblock_socket_;
		}
	}

	if (bind(socket_, (struct sockaddr*) &address_,
			sizeof(struct sockaddr)) < 0)
	{
		return error_binding_;
	}

	if (listen(socket_, c_client_queue_size) < 0)
	{
		return error_start_listen_;
	}

	return error_no_;
}

int simple_server::stop_listen()
{
	int result = shutdown(socket_, SHUT_RDWR);
	close(socket_);
	socket_ = -1;

	if (result < 0)
	{
		return error_shutdown_;
	}
	return error_no_;
}

int simple_server::client_accept(int *client_socket,
	struct sockaddr_in *client_addr) const
{
	socklen_t addr_size = sizeof(struct sockaddr);
	*client_socket = accept(socket_, (struct sockaddr*) client_addr,
			&addr_size);

	if (*client_socket < 0)
	{
		return error_client_accept_;
	}
	return error_no_;
}

int simple_server::get_socket() const
{
	return socket_;
}

} // Net
