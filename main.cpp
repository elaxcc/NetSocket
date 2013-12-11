#include "iostream"

#include "SimpleClient.h"
#include "Parser.h"

int main(int argc, char** argv)
{
	Net::simple_client client(true, true);
	int result = client.connect_to("127.0.0.1", 1234);
	if (result != Net::error_no_)
	{
		std::cout << "Connection fail" << std::endl;
		return 0;
	}
	std::cout << "Connection OK" << std::endl;

	Net::send_data(client.get_socket(), (char *) "out data", 8);

	while(1)
	{
		char buf[150];
		int incoming_data_size;
		int recv_result = Net::recv_data(client.get_socket(),
			buf, 150, &incoming_data_size);
		if (recv_result == Net::error_no_)
		{
			Net::parser parser;
			parser.parse(buf, incoming_data_size);
			if (parser.is_complete())
			{
				HEADER head = parser.get_header();
				std::cout << "CODE: " << head.code << std::endl;
				std::cout << "DATA_LEN: " << head.data_len << std::endl;
				std::cout << "ID: " << head.id << std::endl;
				std::cout << "TV_SEC: " << head.timestamp.tv_sec << std::endl;
				std::cout << "TV_USEC: " << head.timestamp.tv_usec << std::endl;
				std::cout << "VALUE: " << head.value << std::endl;
				std::vector<char> data = parser.get_data();
				for (int i = 0; i < data.size(); ++i)
				{
					std::cout << data[i];
				}
				std::cout << std::endl;
			}
		}
	}

	return 0;
}
