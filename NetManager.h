#ifndef NETMANAGER_H_
#define NETMANAGER_H_

#include <vector>

#include <poll.h>

namespace Net
{

class i_net_member;

/*!
 * Class for net members management
 */
class net_manager
{
public:
	/*!
	 * Constructor
	 */
	net_manager();

	/*!
	 * Destructor
	 */
	~net_manager();

	/*!
	 * Add new net member in mamagement list
	 * [in] member - adding member pointer
	 * retval - socket of added member
	 */
	int add_member(i_net_member *member);

	/*!
	 * Remove net member from management list
	 * [in] socket - deleting net member socket
	 */
	void remove_member(int socket);

	/*!
	 * Process net members after polling
	 * retval error code
	 */
	int process_sockets();

private:
	/*!
	 * Start socket polling
	 * retval error code
	 */
	int poll_sockets(int timeout);

	/*!
	 * Remove net member from polling list
	 */
	void remove_from_polling_list(int socket);

private:
	std::vector<i_net_member*> net_members_;
	std::vector<pollfd> polling_list_;
};

} // namespace Net

#endif /* NETMANAGER_H_ */
