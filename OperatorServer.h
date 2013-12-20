#ifndef OPERATORSERVER_H_
#define OPERATORSERVER_H_

#include "NetCommon.h"

namespace Net
{

class operator_connection : public i_net_member
{
public:
	operator_connection(int socket);

	// i_net_member
	virtual ~operator_connection();
	virtual int process_events(short int polling_events);
	virtual int get_socket();
	virtual short int get_polling_flags();

private:
	int socket_;
};

typedef server<operator_connection> operator_server;

} // namespace Net

#endif /* OPERATORSERVER_H_ */
