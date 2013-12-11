#include "OperatorConnection.h"

namespace Net
{

operator_connection::operator_connection(int socket)
	: socket_(socket)
{
}

operator_connection::~operator_connection()
{
}

int operator_connection::process_event()
{
	return 0;
}

}
