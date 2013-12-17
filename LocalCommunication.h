#ifndef LOCALCOMMUNICATION_H_
#define LOCALCOMMUNICATION_H_

#include <vector>

namespace Net
{

class i_local_communicator
{
public:
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
	virtual int process(int source_id, const std::vector<char>& data) = 0;

	/*!
	 * Set member ID
	 * [in] id - ID for setting
	 */
	void set_id(int id)
	{
		id_ = id;
	}

	/*!
	 * Get member ID
	 * retval - member ID
	 */
	int get_id() const
	{
		return id_;
	}

private:
	int id_;
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
	 * [in] member - communicator pointer for adding
	 * retval - new added communicator ID
	 */
	int add_communicator(i_local_communicator *member);

	/*!
	 * Add message for sending
	 * [in] source_id - message source ID
	 * [in] destination_id - message destination ID
	 * [in] message - message for transmission
	 * retval error code
	 */
	int add_message(int source_id, int destination_id,
		const std::vector<char> message);

	/*!
	 * Process all added messages
	 * retval error code
	 */
	void process();

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
	int current_added_member_id_;
	std::vector<message_info> messages_;
	std::vector<i_local_communicator*> members_;
};

} // namespace Net

#endif /* LOCALCOMMUNICATION_H_ */