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
		User(int socket, const std::string& host, int port, Server *server);

		User(const User& other);

		~User();

		User& operator=(const User& other);

		int port() const;
		int socket() const;

		const std::string& host() const;
		const std::string& nick() const;
		const std::string& message() const;

		size_t response_queue_size() const;
		std::string response_queue_pop();

		void send(const std::string& message);

		void disconnect();
		void append_to_message(const std::string& chunk);

	private:
		void process_message();
		bool is_message_complete() const;
};


}
