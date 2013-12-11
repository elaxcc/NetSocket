#ifndef OPERATORCONNECTION_H_
#define OPERATORCONNECTION_H_

#include "NetCommon.h"
#include "Parser.h"

namespace Net
{

class operator_connection : public i_net_member
{
public:
	operator_connection(int socket);

	// i_net_member
	virtual ~operator_connection();
	virtual int process_event();

private:
	int socket_;
};

}

#endif /* OPERATORCONNECTION_H_ */
