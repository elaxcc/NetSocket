#include "NetManager.h"

namespace Net
{

net_manager::net_manager()
{
}

net_manager::~net_manager()
{
}

void net_manager::remove_member(int socket)
{

}

int net_manager::poll_sockets(short int poll_events, int timeout)
{
	std::vector<pollfd>::iterator iter = polling_list_.begin();
	for (; iter != polling_list_.end(); ++iter)
	{
		iter->events = poll_events;
		iter->revents = 0;
	}

	int result = poll(polling_list_.data(),
		polling_list_.size(), -1);

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

void net_manager::remove_from_polling_list(int socket)
{
	std::vector<pollfd>::iterator iter = polling_list_.begin();
	for (; iter != polling_list_.end(); ++iter)
	{
		if (iter->fd == socket)
		{
			polling_list_.erase(iter);
			return;
		}
	}
}

} // nemaspace Net
