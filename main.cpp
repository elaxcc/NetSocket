#include "iostream"

#include "NetManager.h"
#include "OperatorServer.h"
#include "HardwareClient.h"

int main(int argc, char** argv)
{
	Net::net_manager net_manager;

	//Net::operator_server *operator_server =
	//	new Net::operator_server(&net_manager, 1234, true, true);
	//Net::hardware_client *hardware_client =
	//	new Net::hardware_client(std::string("127.0.0.1"), 1235, true, true);
	//int operator_server_socket = net_manager.add_member(operator_server);
	//int hardware_client_socket = net_manager.add_member(hardware_client);
	//net_manager.remove_member(operator_server_socket);
	//net_manager.remove_member(hardware_client_socket);

	int hardware_client_socket = net_manager.add_member(
		new Net::hardware_client(std::string("127.0.0.1"), 1234, true, true));

	while (true)
	{
		net_manager.process_sockets(POLLIN);
	}

	return 0;
}
