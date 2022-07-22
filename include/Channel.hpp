#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>

#define MODE_DEFAULT	1
#define MODE_T			2
#define MODE_K			4

namespace ft {

class Server;
class User;

//TODO: CHANNEL CHAR LIMIT + USERS LIMIT
class Channel {
	private:
		std::string					_name;
		short						_mode;
		std::string					_key;
		std::string					_topic;
		std::vector<User *>			_users;
		std::vector<User *>			_operators;
		Server						*_server;
		std::vector<std::string>	_ban;

	public:
		Channel(User *creator, std::string name, Server *server);
		Channel(User *creator, std::string name, std::string key, Server *server);
		~Channel();

		const std::string& name() const;
		short	mode(void) const;
		void	mode(short mode);
		void	unset_mode(short mode);

		void	key(std::string key);
		bool	key_compare(std::string key) const;

		const std::string&	topic(void) const;
		void				topic(std::string topic);

		void							ban(const std::string& nick);
		void							unban(const std::string& nick);
		bool							check_banned(const std::string& nick) const;
		const std::vector<std::string>	banned_list(void) const;

		void				add_user(User *user);
		void				remove_user(User *user);
		User*				get_user(const std::string& nick) const;
		bool				user_exist(User *user) const;
		const std::string	list_users(void) const;
		void				update_users(void) const;
		size_t				count_users(void) const;

		void	add_operator(User *op);
		void	remove_operator(User *op);
		bool	check_operator(User *user) const;
		void	op(const std::string& nick);
		void	deop(const std::string& nick);

		void	dispatch_message(User *sender, const std::string& message) const;

	private:
		std::vector<User *>::iterator				find_user(User *);
		std::vector<User *>::const_iterator			find_user(User *) const;
		std::vector<User *>::iterator				find_operator(User *);
		std::vector<User *>::const_iterator			find_operator(User *) const;
		std::vector<std::string>::iterator			find_banned(const std::string&);
		std::vector<std::string>::const_iterator	find_banned(const std::string&) const;
};

}
