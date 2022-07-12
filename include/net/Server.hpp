#pragma once

#include "TCPSocket.hpp"
#include "Client.hpp"
#include <map>
#include <string>
#include <vector>

namespace ft {

class Server {
	private:
		TCPSocket _socket;

		std::vector<Client *> _clients;

	public:
		Server();
		Server(TCPSocket socket);
		Server(const Server& server);
		~Server();
		Server& operator=(const Server& other);
		TCPSocket socket();

		void close();
		void handle_new_connections();
		void poll();
};

}
