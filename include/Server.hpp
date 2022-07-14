#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <arpa/inet.h>

#include "Commands.hpp"

namespace ft {

class User;

class Server {
	private:
		std::vector<User *> 						_users;
		std::vector<struct pollfd>					_pollfds;
		std::map<std::string, Command *>			_commands;
		int											_socket;

	public:
		Server(int port);
		~Server();
		void poll();
		void disconnect(size_t user_index);

		void process_message(User& sender, const std::string& message);
		uint64_t find_user_index(const User& user);

	private:
		void parse_message(const std::string& message, std::string& command, std::vector<std::string>& params);
		void setup_commands();
};

}
