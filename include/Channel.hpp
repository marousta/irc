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
		short				_mode;
		std::string			_key;
		std::string			_topic;
		std::vector<User *>	_users;
		std::vector<User *>	_operators;

	public:
		Channel(User *creator, std::string name);
		Channel(User *creator, std::string name, std::string key);
		~Channel();

		short	mode(void) const;
		void	mode(short mode);
		void	unset_mode(short mode);

		const std::string&	key(void) const;

		const std::string&	topic(void) const;
		void				topic(std::string topic);

		void	add_user(User *user);
		void	remove_user(User *user);

		void	add_operator(User *op);
		void	remove_operator(User *op);

		void	dispatch_message(User *sender, std::string message);

	private:
		std::vector<User *>::iterator find_user(User *);
		std::vector<User *>::iterator find_operator(User *);
};

}
