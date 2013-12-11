#ifndef SIMPLESERVER_H_
#define SIMPLESERVER_H_

#include "NetCommon.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace Net
{

class simple_server
{
public:
	simple_server(bool nonblocking, bool no_nagle_delay);
	~simple_server();
	int start_listen(int port);
	int stop_listen();
	int client_accept(int *client_socket,
		struct sockaddr_in *client_addr) const;
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

} // Net

#endif /* SIMPLESERVER_H_ */
