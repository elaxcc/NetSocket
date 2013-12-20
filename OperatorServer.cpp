#include "OperatorServer.h"

#include "NetManager.h"

namespace Net
{

operator_connection::operator_connection(int socket)
	: socket_(socket)
{
}

operator_connection::~operator_connection()
{
}

int operator_connection::process_events(
	short int polling_events)
{
	return 0;
}

int operator_connection::get_socket()
{
	return socket_;
}

short int operator_connection::get_polling_flags()
{
	return POLLOUT;
}

} // Net
