#include "NetManager.h"

namespace Net
{

namespace
{

const int c_infinum_timeout = -1;
const short c_showed_polling_flags = POLLIN | POLLOUT | POLLPRI;

}

net_manager::net_manager()
{
}

net_manager::~net_manager()
{
	for (unsigned int i = 0; i < net_members_.size(); ++i)
	{
		delete net_members_[i];
	}

	net_members_.clear();
	polling_list_.clear();
}

int net_manager::add_member(i_net_member *member)
{
	if (member)
	{
		net_members_.push_back(member);

		pollfd fd = {member->get_socket(), 0, 0};
		polling_list_.push_back(fd);
	}

	return member->get_socket();
}

void net_manager::remove_member(int socket)
{
	std::vector<i_net_member*>::iterator member_iter =
		net_members_.begin();
	for(; member_iter != net_members_.end(); ++member_iter)
	{
		if ((*member_iter)->get_socket() == socket)
		{
			delete (*member_iter);
			net_members_.erase(member_iter);
			remove_from_polling_list(socket);

			return;
		}
	}
}

int net_manager::process_sockets()
{
	int poll_result = poll_sockets(c_showed_polling_flags,
		c_infinum_timeout);

	if (poll_result == error_no_)
	{
		for (unsigned int i = 0; i < polling_list_.size(); ++i)
		{
			short int member_polling_flags =
				net_members_[i]->get_polling_flags();
			if ((member_polling_flags != 0)
				&& (polling_list_[i].revents & member_polling_flags))
			{
				net_members_[i]->process_events(polling_list_[i].revents);
			}
		}
	}

	return poll_result;
}

int net_manager::poll_sockets(short int poll_events, int timeout)
{
	for (unsigned int i = 0; i < polling_list_.size(); ++i)
	{
		polling_list_[i].events = net_members_[i]->get_polling_flags();
		polling_list_[i].revents = 0;
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
