#pragma once

#include <vector>
#include <arpa/inet.h>
#include "User.hpp"

namespace ft {

class Server {
	private:
		std::vector<User *> 		_users;
		std::vector<struct pollfd>	_pollfds;
		int							_socket;

	public:
		Server(int port);
		~Server();
		void poll();
		void disconnect(size_t user_index);
};

}
