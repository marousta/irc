#pragma once

#include "TCPSocket.hpp"
#include "Client.hpp"
#include <map>
#include <string>

namespace ft {

class Server {
	private:
		TCPSocket _socket;

		// Map of vectors because multiple users might be on the same IP Address
		std::map<std::string, Client> _clients;

	public:
		Server();
		Server(TCPSocket socket);
		Server(const Server& server);
		~Server();
		Server& operator=(const Server& other);
		TCPSocket socket();

		void close();
};

}
