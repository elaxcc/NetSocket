#ifndef OPERATORCONNECTION_H_
#define OPERATORCONNECTION_H_

#include "NetCommon.h"
#include "OperatorServer.h"

namespace Net
{

class operator_server::operator_connection : public i_net_member
{
public:
	operator_connection(int socket);

	// i_net_member
	virtual ~operator_connection();
	virtual int process_event();
	virtual int get_socket();

private:
	int socket_;
};

}

#endif /* OPERATORCONNECTION_H_ */
