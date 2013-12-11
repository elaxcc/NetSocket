#include "HardwareClient.h"

#include "iostream"

namespace Net
{

hardware_client::hardware_client(const std::string& address, int port,
	bool nonblocking, bool no_nagle_delay)
	: simple_client(nonblocking, no_nagle_delay)
{
	connect_to(address, port);

	Net::send_data(simple_client::get_socket(), (char *) "out data", 8);
}

hardware_client::~hardware_client()
{
	close_connection();
}

int hardware_client::process_events(short int polling_events)
{
	if (polling_events & POLLIN)
	{
		char buf[150];
		int incoming_data_size;
		int recv_result = Net::recv_data(simple_client::get_socket(),
			buf, 150, &incoming_data_size);
		if (recv_result == Net::error_no_)
		{
			parser_.parse(buf, incoming_data_size);

			HEADER head = parser_.get_header();
			std::cout << "CODE: " << head.code << std::endl;
			std::cout << "DATA_LEN: " << head.data_len << std::endl;
			std::cout << "ID: " << head.id << std::endl;
			std::cout << "TV_SEC: " << head.timestamp.tv_sec << std::endl;
			std::cout << "TV_USEC: " << head.timestamp.tv_usec << std::endl;
			std::cout << "VALUE: " << head.value << std::endl;
			std::vector<char> data = parser_.get_data();
			for (unsigned int i = 0; i < data.size(); ++i)
			{
				std::cout << data[i];
			}
			std::cout << std::endl;
		}
	}

	return error_no_;
}

int hardware_client::get_socket()
{
	return simple_client::get_socket();
}

short int hardware_client::get_polling_flags()
{
	return POLLIN;
}

} // Net
