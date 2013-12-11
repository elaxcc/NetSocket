#include "NetCommon.h"

#include <netinet/tcp.h>
#include <poll.h>
#include <errno.h>
#include <string.h>

namespace Net
{

int send_data(int socket, char *data, int data_size)
{
	int sended_bytes;
		int total_bytes = data_size;

		while(total_bytes)
		{
			sended_bytes = send(
					socket,
					&(data[data_size - total_bytes]),
					total_bytes, 0);

			if (sended_bytes < 0)
			{
				return error_send_;
			}

			total_bytes -= sended_bytes;
		}
		return error_no_;
}

int recv_data(int socket, char *buf, int buf_size, int *recv_data_size)
{

	*recv_data_size = recv(socket, buf, buf_size, 0);

	if (*recv_data_size == 0)
	{
		return error_connection_is_closed_;
	}
	else if (*recv_data_size < 0)
	{
		return error_recv_;
	}
	return error_no_;
}

int wait_socket(int socket, short int poll_event, int poll_timeout)
{
	struct pollfd fd;
	memset(&fd, 0, sizeof(fd));
	fd.fd = socket;
	fd.events = poll_event;

	int result = poll(&fd, 1, poll_timeout);

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

} // Net
