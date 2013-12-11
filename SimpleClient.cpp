#include "SimpleClient.h"

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

const int c_poll_timeout = 1000;

}

namespace Net
{

simple_client::simple_client(bool nonblocking, bool no_nagle_delay)
	: socket_(-1)
	, nonblocking_(nonblocking)
	, no_nagle_delay_(no_nagle_delay)
{
}

simple_client::~simple_client()
{
	if (socket_ > 0)
	{
		shutdown(socket_, SHUT_RDWR);
		close(socket_);
	}
}

int simple_client::connect_to(const std::string& address, int port)
{
	address_.sin_family = AF_INET;
	address_.sin_port = htons(port);
	address_.sin_addr.s_addr = inet_addr(address.c_str());

	// отключить алгоритм нагла

	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ < 0)
	{
		return error_create_socket_;
	}

	if (no_nagle_delay_)
	{
		int set_opt_val = 1;
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

	int connect_result = connect(socket_, (struct sockaddr *) &address_,
			sizeof(struct sockaddr));
	if (nonblocking_ && connect_result == -1 && errno != EINPROGRESS)
	{
		return error_connect_;
	}
	return error_no_;
}

int simple_client::close_connection()
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

int simple_client::get_socket() const
{
	return socket_;
}

} // Net
