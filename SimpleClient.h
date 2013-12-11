#ifndef SIMPLECLIENT_H_
#define SIMPLECLIENT_H_

#include "NetCommon.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "string"

namespace Net
{

class simple_client
{
public:
	simple_client(bool nonblocking, bool no_nagle_delay);
	~simple_client();
	int connect_to(const std::string& address, int port);
	int close_connection();
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

} // Net

#endif /* SIMPLECLIENT_H_ */
