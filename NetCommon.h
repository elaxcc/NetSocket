#ifndef NETCOMMON_H_
#define NETCOMMON_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "string"

namespace Net
{

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
	error_can_not_find_server_ = -17
};

class i_net_member
{
public:
	virtual ~i_net_member() {};
	virtual int process_event() = 0;
	virtual int get_socket() = 0;
};

class simple_client
{
public:
	simple_client(bool nonblocking, bool no_nagle_delay);
	~simple_client();
	int connect_to(const std::string& address, int port);
	int close_connection();
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

class simple_server
{
public:
	simple_server(bool nonblocking, bool no_nagle_delay);
	~simple_server();
	int start_listen(int port);
	int stop_listen();
	int client_accept(int *client_socket,
		struct sockaddr_in *client_addr) const;
	int get_socket() const;

private:
	int socket_;
	struct sockaddr_in address_;
	bool nonblocking_;
	bool no_nagle_delay_;
};

int send_data(int socket, char *data, int data_size);
int recv_data(int socket, char *buf, int buf_size, int *recv_data_size);
int wait_socket(int socket, short int poll_event, int poll_timeout);

}

#endif /* NETCOMMON_H_ */
