#include "OperatorConnection.h"

namespace Net
{

operator_server::operator_connection::operator_connection(int socket)
	: socket_(socket)
{
}

operator_server::operator_connection::~operator_connection()
{
}

int operator_server::operator_connection::process_events(
	short int polling_events)
{
	return 0;
}

int operator_server::operator_connection::get_socket()
{
	return socket_;
}

short int operator_server::operator_connection::get_polling_flags()
{
	return POLLOUT;
}

} // namespace Net
