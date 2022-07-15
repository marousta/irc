#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <arpa/inet.h>

#include "Commands.hpp"
#include "Channel.hpp"

namespace ft {

class User;

class Server {
	private:
		std::vector<User *> 						_users;
		std::vector<struct pollfd>					_pollfds;
		std::map<std::string, Command *>			_commands;
		std::map<std::string, Channel *>			_channels;
		int											_socket;

	public:
		Server(int port);
		~Server();

		void	poll();
		void	disconnect(size_t user_index);

		void	process_message(User& sender, const std::string& message);
		int  	find_user_index(const User& user);

		Channel*	get_channel(std::string name);
		void	create_channel(User *creator, const std::vector<std::string>& args);
		void	delete_channel();

	private:
		void parse_message(const std::string& message, std::string& command, std::vector<std::string>& params);
		void setup_commands();
		void send_error(User& user, const std::string& err);
};

}
