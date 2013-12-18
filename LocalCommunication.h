#ifndef LOCALCOMMUNICATION_H_
#define LOCALCOMMUNICATION_H_

#include <string>
#include <vector>
#include <map>

namespace Net
{

class local_communicator_manager;

class i_local_communicator
{
public:
	/*!
	 * Constructor
	 * [in] unique_id - unique id of communicator
	 */
	i_local_communicator(local_communicator_manager *manager);

	/*!
	 * Destructor
	 */
	virtual ~i_local_communicator();

	/*!
	 * Process message
	 * [in] source_id - message source ID
	 * [in] data - message data
	 * retval error code
	 */
	virtual int process_message(const std::string& link,
		const std::vector<char>& data) = 0;

public:
	/*!
	 * Get member ID
	 * retval - member ID
	 */
	int get_id() const;

	/*!
	 * Transmit massage by link
	 * [in] link - link name
	 * [in] message - message for transmission
	 * retval - transmission result
	 */
	bool send_message(const std::string& link,
		const std::vector<char>& message);

private:
	/*!
	 * Set communicator ID
	 * [in] id - ID for setting
	 */
	void set_id(int id);

	/*!
	 * Add link to links list
	 * [in] link - link name
	 * [in] destination_id - destination ID for linking
	 */
	void add_link(const std::string& link, int destination_id);

	/*!
	 * Remove link from list
	 * [in] link - link name for removing
	 */
	void remove_link(const std::string& link);

	/*!
	 * Find link in list by destination ID
	 * [in] id - member ID for searching
	 * retval - link
	 */
	std::string find_link_by_id(int id) const;

private:
	friend class local_communicator_manager;

private:
	int id_;
	std::map<std::string, int> links_list_;
	local_communicator_manager* manager_;
};

class local_communicator_manager
{
public:
	/*!
	 * Constructor
	 */
	local_communicator_manager();

	/*!
	 * Destructor
	 */
	~local_communicator_manager();

	/*!
	 * [in] link_name - name of the new link, can not be zero string
	 * [in] member_first - first member for linking
	 * [in] member_second - second member for linking
	 */
	bool create_link(const std::string& link_name,
		i_local_communicator* member_first,
		i_local_communicator* member_second);

	/*!
	 * Process all added messages
	 */
	void process();

private:
	/*!
	 * [in] member - communicator pointer for adding
	 * retval - new added communicator ID
	 */
	void add_communicator(i_local_communicator *member);

	/*!
	 * Add message for sending
	 * [in] source_id - message source ID
	 * [in] destination_id - message destination ID
	 * [in] message - message for transmission
	 * retval error code
	 */
	bool add_message(int source_id, int destination_id,
		const std::vector<char>& message);

private:
	friend class i_local_communicator;

private:
	/*!
	 * Message information
	 */
	struct message_info
	{
		int source_;
		int destination_;
		std::vector<char> message_;

		message_info(int source, int destination,
			const std::vector<char> message)
			: source_(source)
			, destination_(destination)
			, message_(message)
		{
		}

		~message_info()
		{
			message_.clear();
		}
	};

private:
	std::vector<message_info> messages_;
	std::map<int, i_local_communicator*> members_;
	int last_added_member_id_;
};

} // namespace Net

#endif /* LOCALCOMMUNICATION_H_ */
