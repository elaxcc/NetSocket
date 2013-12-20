#ifndef NETCOMMON_H_
#define NETCOMMON_H_

#include "NetManager.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#include "string"

namespace Net
{

/*!
 * List of net errors
 */
enum error
{
	error_no_ = 0,
	error_create_socket_ = -1,
	error_binding_ = -2,
	error_create_nonblock_socket_ = -3,
	error_start_listen_ = -4,
	error_shutdown_ = -5,
	error_set_opt_reuse_addr_ = -6,
	error_set_opt_nodelay_ = -7,
	error_client_accept_ = -8,
	error_send_ = -9,
	error_recv_ = -10,
	error_connection_is_closed_ = -11,
	error_poll_timeout_ = -12,
	error_poll_ = -13,
	error_connect_ = -14,
	error_wrong_net_member_type_ = -15,
	error_can_not_find_socket_ = -16,
	error_can_not_find_server_ = -17,
	error_server_pollin_no_connect_co_data_ = -18
};

/*!
 * common net member interface
 */
class i_net_member
{
public:
	virtual ~i_net_member() {};

	/*!
	 * Process polling events that installed in 'polling_events'
	 * [in] polling_events - polling flags that has been occurred
	 * retval - error code
	 */
	virtual int process_events(short int polling_events) = 0;

	/*!
	 * Return socket of the current net member
	 * retval - net member socket
	 */
	virtual int get_socket() = 0;

	/*!
	 * Return polling flags that used by this net member in that time
	 * retval - polling flags
	 */
	virtual short int get_polling_flags() = 0;
};

/*!
 * Class for client realization
 */
class simple_client
{
public:
	/*!
	 * Constructor
	 * [in] nonblocking - flag showed to create nonblocked socket
	 * [in] no_nagle_delay -flag shoed to create socket without
	 * Nagle's algorithm
	 */
	simple_client(bool nonblocking, bool no_nagle_delay);

	/*!
	 * Destructor
	 */
	~simple_client();

	/*!
	 * Connect to server
	 * [in] address - address of the server for connection
	 * [in] port - server port for connection
	 * retval - error code
	 */
	int connect_to(const std::string& address, int port);

	/*!
	 * Close connection
	 * retval - error code
	 */
	int close_connection();

	/*!
	 * Return own client socket
	 * retval - own socket
	 */
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

/*!
 * Class for server realization
 */
class simple_server
{
public:
	/*!
	 * Constructor
	 * [in] nonblocking - flag showed to create nonblocked socket
	 * [in] no_nagle_delay -flag shoed to create socket without
	 * Nagle's algorithm
	 */
	simple_server(bool nonblocking, bool no_nagle_delay);

	/*!
	 * Destructor
	 */
	~simple_server();

	/*!
	 * Start listen clients
	 * [in] port - port for listening
	 * retval - error code
	 */
	int start_listen(int port);

	/*!
	 * Stop listen clients
	 * retval - error code
	 */
	int stop_listen();

	/*!
	 * Accept new client
	 * [out] client_socket - new client socket
	 * [out] client_addr - new client address
	 * retval - error code
	 */
	int client_accept(int *client_socket,
		struct sockaddr_in *client_addr) const;

	/*!
	 * Return socket
	 * retval - own socket
	 */
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

template<class connection>
class server : public i_net_member, private simple_server
{
public:
	server(net_manager *net_manager,
		int port, bool nonblocking, bool no_nagle_delay)
		: simple_server(nonblocking, no_nagle_delay)
		, net_manager_(net_manager)
	{
		start_listen(port);
	}

	~server()
	{
		stop_listen();
	}

	int process_events(short int polling_events)
	{
		int accept_result = error_no_;
		if (polling_events & POLLIN)
		{
			int new_client_socket;
			struct sockaddr_in new_client_addr;

			accept_result = client_accept(
				&new_client_socket, &new_client_addr);

			if (accept_result == error_no_)
			{
				net_manager_->add_member(
					new connection(new_client_socket));
			}
		}
		return accept_result;
	}

	int get_socket()
	{
		return simple_server::get_socket();
	}

	short int get_polling_flags()
	{
		return POLLIN;
	}

private:
	net_manager *net_manager_;
};

/*!
 * Send data to socket
 * [in] socket - socket for sending
 * [in] data - data for sending
 * [in] data_size - data size for sending
 * retval - error code
 */
int send_data(int socket, char *data, int data_size);

/*!
 * Receive data from socket
 * [in] socket - socket for receiving
 * [out] buf - buffer for storing data
 * [in] buf_size - buffer size for receiving
 * retval - error code
 */
int recv_data(int socket, char *buf, int buf_size, int *recv_data_size);

} // namespace Net

#endif /* NETCOMMON_H_ */
