#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <map>

#include "Channel.hpp"
#include "Commands.hpp"

namespace ft {

class User;

class Server {
	private:
		std::string									_pass;
		std::vector<User *> 						_users;
		std::vector<struct pollfd>					_pollfds;
		std::map<std::string, Command *>			_commands;
		std::map<std::string, Channel *>			_channels;
		int											_socket;

	public:
		Server(int port = 6667, std::string pass = "");
		~Server();

		bool	check_pass(std::string pass) const;

		void	poll();
		void	disconnect(size_t user_index);

		void	process_message(User& sender, const std::string& message);

		int  	find_user_index(const User& user);
		User*	get_user_username(const std::string& username) const;
		User*	get_user_nick(const std::string& nick) const;
		size_t	user_count(void) const;

		void		create_channel(User *creator, const std::vector<std::string>& args);
		void		delete_channel();
		void		join_channel(User *user, const std::vector<std::string>& args);
		Channel*	get_channel(const std::string& name);
		size_t		channel_count(void) const;

		void	print_debug(User *sender) const;

	private:
		int		find_user_username(const std::string& username) const;
		int		find_user_nick(const std::string& nick) const;

		void 	parse_message(const std::string& message, std::string& command, std::vector<std::string>& params);
		void 	setup_commands();
		void 	send_error(User& user, const std::string& err);
};

}
