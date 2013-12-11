#ifndef HARDWARECLIENT_H_
#define HARDWARECLIENT_H_

#include "NetCommon.h"
#include "Parser.h"

namespace Net
{

class hardware_client : public i_net_member, private simple_client
{
public:
	hardware_client(const std::string& address, int port,
		bool nonblocking, bool no_nagle_delay);

	// i_net_member
	virtual ~hardware_client();
	virtual int process_event();
	virtual int get_socket();

private:
	parser parser_;
};

} // Net

#endif /* HARDWARECLIENT_H_ */
