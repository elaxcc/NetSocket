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

int operator_server::operator_connection::process_event()
{
	return 0;
}

int operator_server::operator_connection::get_socket()
{
	return socket_;
}

} // namespace Net