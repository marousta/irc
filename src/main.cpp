#include "net/Server.hpp"
#include <iostream>
#include <vector>

int main()
{
	ft::TCPSocket server_socket;

	try {
		server_socket = ft::TCPSocketBuilder()
			.port(8080)
			.build();
		std::cout <<"created server socket" << std::endl;
	} catch (std::exception& e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
	}

	ft::Server server(server_socket);

	std::vector<ft::Client> clients;

	for (;;) {
		ft::Client client;
		do {
			client = server.socket().accept();
			if (client.is_valid()) {
				clients.push_back(client);
				std::cout << "Client connected" << std::endl;
			}
		} while (client.is_valid());
	}

	server.close();
}
