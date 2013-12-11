#include "HardwareClient.h"

namespace Net
{

hardware_client::hardware_client(const std::string& address, int port,
	bool nonblocking, bool no_nagle_delay)
	: simple_client(nonblocking, no_nagle_delay)
{
	connect_to(address, port);
}

hardware_client::~hardware_client()
{
	close_connection();
}

int hardware_client::process_event()
{
	return 0;
}

} // Net
