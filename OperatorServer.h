#ifndef OPERATORSERVER_H_
#define OPERATORSERVER_H_

#include "NetCommon.h"

namespace Net
{

class net_manager;

class operator_server : public i_net_member, private simple_server
{
public:
	operator_server(net_manager *net_manager, int port,
		bool nonblocking, bool no_nagle_delay);

	// i_net_member
	virtual ~operator_server();
	virtual int process_event();
	virtual int get_socket();

private:
	class operator_connection;

private:
	net_manager *net_manager_;
};

} // Net

#endif /* OPERATORSERVER_H_ */
