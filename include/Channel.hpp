#pragma once

#include <iostream>
#include <vector>
#include <stdint.h>

#define MODE_DEFAULT	0
#define MODE_T			2
#define MODE_K			4

namespace ft {

class User;

class Channel {
	private:
		std::string			_name;
		uint32_t			_mode;
		std::string			_key;
		std::string			_topic;
		std::vector<User *>	_users;
		std::vector<User *>	_operators;

	public:
		Channel(User *creator, std::string name);
		Channel(User *creator, std::string name, std::string key);
		~Channel();

		void	set_mode(uint32_t mode);
		void	unset_mode(uint32_t mode);

		void	set_topic(std::string topic);

		void	add_user(User *user);
		void	remove_user(User *user);

		void	add_operator(User *user);
		void	remove_operator(User *user);

		void	dispatch_message(User *sender, std::string message);
};

}
