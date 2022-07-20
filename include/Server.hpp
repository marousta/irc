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
		std::vector<std::string>					_ignored_commands;
		std::map<std::string, Channel *>			_channels;
		std::vector<User *>							_disconnect_requests;
		int											_socket;

	public:
		Server(int port = 6667, std::string pass = "");
		~Server();

		bool	check_pass(std::string pass) const;

		void	poll();
		void	request_disconnect(size_t user_index);
		void	disconnect(size_t user_index);

		void	process_message(User& sender, const std::string& message);

		int  	find_user_index(const User& user);
		User*	get_user_username(const std::string& username) const;
		User*	get_user_nick(const std::string& nick) const;
		size_t	user_count(void) const;

		void	create_channel(User *creator, const std::string& name, const std::string& key = "");
		void	remove_channel(const std::string& name);
		void	join_channel(User *user, const std::string& name, const std::string& key = "");
		size_t	channel_count(void) const;

		Channel*				get_channel_with_name(const std::string& name);
		std::vector<Channel *> 	get_channels() const;
		std::vector<Channel *> 	get_channels_with_user(User *user);

		void	print_debug(User *sender) const;

		std::map<std::string, Command *>	get_commands() const;

	private:
		int		find_user_username(const std::string& username) const;
		int		find_user_nick(const std::string& nick) const;

		void 	setup_commands();
		void 	setup_ignored_commands();
		void 	send_error(User& user, const std::string& err);
};

}
