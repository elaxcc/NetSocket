#include "LocalCommunication.h"

namespace Net
{

i_local_communicator::i_local_communicator(
	local_communicator_manager *manager)
	: id_(0)
	, manager_(manager)
{
}

i_local_communicator::~i_local_communicator()
{
	links_list_.clear();
}

int i_local_communicator::get_id() const
{
	return id_;
}

bool i_local_communicator::send_message(const std::string& link,
	const std::vector<char>& message)
{
	std::map<std::string, int>::iterator iter =
		links_list_.find(link);

	if (iter == links_list_.end())
	{
		return false;
	}

	manager_->add_message(id_, iter->second,
		message);

	return true;
}

void i_local_communicator::set_id(int id)
{
	id_ = id;
}

void i_local_communicator::add_link(
	const std::string& link, int destination_id)
{
	std::map<std::string, int>::iterator iter =
		links_list_.find(link);
	if (iter == links_list_.end())
	{
		links_list_.insert(
			std::pair<std::string, int>(
				link, destination_id));
	}
}

void i_local_communicator::remove_link(const std::string& link)
{
	std::map<std::string, int>::iterator iter =
		links_list_.find(link);
	if (iter != links_list_.end())
	{
		links_list_.erase(iter);
	}
}

std::string i_local_communicator::find_link_by_id(int id) const
{
	std::map<std::string, int>::const_iterator iter =
		links_list_.begin();

	for (; iter != links_list_.end(); ++iter)
	{
		if (iter->second == id)
		{
			return iter->first;
		}
	}

	return std::string();
}

local_communicator_manager::local_communicator_manager()
	: last_added_member_id_(0)
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
			last_added_member_id_++;
			member->set_id(last_added_member_id_);

			members_.insert(
				std::pair<int, i_local_communicator*>(
						member->get_id(), member));
		}
	}
}

bool local_communicator_manager::create_link(
	const std::string& link_name,
	i_local_communicator* member_first,
	i_local_communicator* member_second)
{
	if (link_name.empty() || !member_first || !member_second)
	{
		return false;
	}

	add_communicator(member_first);
	add_communicator(member_second);

	member_first->add_link(link_name, member_second->get_id());
	member_second->add_link(link_name, member_first->get_id());

	return true;
}

bool local_communicator_manager::add_message(
	int source_id, int destination_id,
	const std::vector<char>& message)
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
			std::string link = iter->second->find_link_by_id
				(messages_[i].source_);

			if (iter->second)
			{
				iter->second->process_message(link,
					messages_[i].message_);
			}
		}
	}

	messages_.clear();
}

} // namespace Net
