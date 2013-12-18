#include "LocalCommunication.h"

namespace Net
{

local_communicator_manager::local_communicator_manager()
{
}

local_communicator_manager::~local_communicator_manager()
{
	messages_.clear();
	members_.clear();
}

void local_communicator_manager::add_communicator(
	i_local_communicator *member)
{
	if (member)
	{
		std::map<int, i_local_communicator*>::iterator iter =
			members_.find(member->get_id());
		if (iter == members_.end())
		{
			members_.insert(std::pair<int, i_local_communicator*>(
				member->get_id(), member));
		}
	}
}

bool local_communicator_manager::add_message(
	int source_id, int destination_id,
	const std::vector<char> message)
{
	std::map<int, i_local_communicator*>::iterator iter =
		members_.find(destination_id);

	if (iter != members_.end())
	{
		messages_.push_back(
			message_info(
				source_id,
				destination_id,
				message));
		return true;
	}
	return false;
}

void local_communicator_manager::process()
{
	for (unsigned int i = 0; i < messages_.size(); ++i)
	{
		std::map<int, i_local_communicator*>::iterator iter =
			members_.find(messages_[i].destination_);

		if (iter != members_.end())
		{
			iter->second->process_message(messages_[i].source_,
				messages_[i].message_);
		}
	}

	messages_.clear();
}

} // namespace Net
