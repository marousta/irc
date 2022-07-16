#pragma once

#include <string>
#include <queue>

namespace ft {

class Server;

class User {
	private:
		bool					_did_enter;
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
		User(int socket, const std::string& host, int port, Server *server, bool enter);
		User(const User& other);
		~User();

		User& operator=(const User& other);

		int port() const;
		int socket() const;
		bool entered() const;
		void entered(bool state);

		const std::string& host() const;
		const std::string& nick() const;
		const std::string& username() const;
		const std::string& message() const;

		size_t response_queue_size() const;
		std::string response_queue_pop();

		void send(const std::string& message);

		bool registered() const;
		void disconnect();
		void append_to_message(const std::string& chunk);

	private:
		bool check_register() const;

		void process_message();
		bool is_message_complete() const;
};


}
