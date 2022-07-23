#pragma once

#include <string>
#include <queue>

namespace ft {

class Server;

class User {
	private:
		bool					_did_enter;
		bool					_did_register;
		int						_socket;
		int						_port;
		std::string				_host;
		std::string				_nick;
		std::string				_username;
		std::string				_realname;
		std::string				_message;
		Server					*_server;
		std::queue<std::string>	_response_queue;

	public:
		/* socket = fd */
		/* host = ip */
		/* enter = password needed or not */
		User(int socket, const std::string& host, int port, Server *server, bool enter);
		User(const User& other);
		~User();

		User& operator=(const User& other);

		int		port() const;
		int		socket() const;
		bool	entered() const;
		void	entered(bool state);
		bool	registered() const;

		bool	check_register();

		const std::string& host() const;

		const std::string&	nick() const;
		void				nick(std::string& nick);

		const std::string&	username() const;
		void				username(std::string& username);
		const std::string&	realname() const;
		void				realname(std::string& realname);

		size_t		response_queue_size() const;
		std::string	response_queue_pop();

		void send(const std::string& message);

		void disconnect();
		void append_to_message(const std::string& chunk);

	private:
		void process_message();
		bool is_message_complete() const;
};


}
