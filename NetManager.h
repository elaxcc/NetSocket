#ifndef NETMANAGER_H_
#define NETMANAGER_H_

#include "NetCommon.h"

#include <vector>

#include <poll.h>

namespace Net
{

class net_manager
{
public:
	net_manager();
	~net_manager();

	int add_member(i_net_member *member);
	void remove_member(int socket);

	int process_sockets(short int polling_flags);

private:
	int poll_sockets(short int poll_events, int timeout);
	void remove_from_polling_list(int socket);

private:
	std::vector<i_net_member*> net_members_;
	std::vector<pollfd> polling_list_;
};

} // name

#endif /* NETMANAGER_H_ */
