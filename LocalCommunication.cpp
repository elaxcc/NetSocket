#include "LocalCommunication.h"

namespace Net
{

local_communicator_manager::local_communicator_manager()
	: current_added_member_id_(0)
{
}

local_communicator_manager::~local_communicator_manager()
{
	messages_.clear();
	members_.clear();
}

int local_communicator_manager::add_communicator(
	i_local_communicator *member)
{
	if (member)
	{
		current_added_member_id_++;
		member->set_id(current_added_member_id_);
		members_.insert(std::pair(current_added_member_id_,
			member));

		return current_added_member_id_;
	}

	return 0;
}

int local_communicator_manager::add_message(
	int source_id, int destination_id,
	const std::vector<char> message)
{
	std::map<int, i_local_communicator*>::iterator iter =
		members_.find(destination_id);

	if (iter != members_.end())
	{
		if (iter->second->get_id() == destination_id)
		{
			messages_.push_back(
				message_info(
					source_id,
					destination_id,
					message));
			return 0;
		}
	}
	return -1;
}

void local_communicator_manager::process()
{
	for (unsigned int i = 0; i < messages_.size(); ++i)
	{
		std::map<int, i_local_communicator*>::iterator iter =
			members_.find(messages_[i].destination_);

		if (iter != members_.end())
		{
			iter->second->process(messages_[i].source_,
				messages_[i].message_);
		}
	}

	messages_.clear();
}

} // namespace Net
