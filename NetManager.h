#ifndef NETMANAGER_H_
#define NETMANAGER_H_

#include "NetCommon.h"

#include <vector>
#include <map>
#include <memory>

#include <poll.h>

namespace Net
{

class net_manager
{
public:
	net_manager();
	~net_manager();

	//void add_member(const std::shared_ptr<i_net_member>& member);
	void remove_member(int socket);

	int poll_sockets(short int poll_events, int timeout);

private:
	void remove_from_polling_list(int socket);

private:
	std::map<int, int > members_list_;
	std::vector<pollfd> polling_list_;
};

} // name

#endif /* NETMANAGER_H_ */
