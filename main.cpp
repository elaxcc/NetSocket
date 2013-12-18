#include "iostream"

#include "NetManager.h"
#include "OperatorServer.h"
#include "HardwareClient.h"
#include "LocalCommunication.h"

int main(int argc, char** argv)
{
	/*
	Net::net_manager net_manager;
	Net::local_communicator_manager local_communicator_;

	int hardware_client_socket = net_manager.add_member(
		new Net::hardware_client(std::string("127.0.0.1"), 1234, true, true));

	while (true)
	{
		net_manager.process_sockets();
		// process local messages
		local_communicator_.process();
	}
	*/

	class A : public Net::i_local_communicator
	{
	public :
		A(Net::local_communicator_manager *manager)
			: i_local_communicator(manager)
		{
		}

		virtual int process_message(const std::string& link,
		const std::vector<char>& data)
		{
			std::cout << "I am A, my data: ";
			for (unsigned int i = 0; i < data.size(); ++i)
			{
				std::cout << data[i];
			}
			std::cout << std::endl;

			return 0;
		}
	};

	class B : public Net::i_local_communicator
	{
	public :
		B(Net::local_communicator_manager *manager)
			: i_local_communicator(manager)
		{
		}

		virtual int process_message(const std::string& link,
		const std::vector<char>& data)
		{
			std::cout << "I am B, my data: ";
			for (unsigned int i = 0; i < data.size(); ++i)
			{
				std::cout << data[i];
			}
			std::cout << std::endl;

			return 0;
		}
	};

	Net::local_communicator_manager local_comm_manager;
	A a(&local_comm_manager);
	B b(&local_comm_manager);

	local_comm_manager.create_link("AB", &a, &b);

	char a_data[] = "Hello B";
	std::vector<char> a_vec(
		a_data, a_data + sizeof(a_data) / sizeof(char));
	a.send_message("AB", a_vec);

	char b_data[] = "Hello A";
	std::vector<char> b_vec(
		b_data, b_data + sizeof(b_data) / sizeof(char));
	b.send_message("AB", b_vec);

	while (true)
	{
		local_comm_manager.process();
	}

	return 0;
}
