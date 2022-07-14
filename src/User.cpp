#include "Server.hpp"
#include "User.hpp"
#include <iostream>
#include <unistd.h>

namespace ft {

User::User(int socket, const std::string& host, int port, Server *server)
	:	_did_enter(false), _did_register(false), _socket(socket), _port(port),
		_host(host), _nick(""), _username(""), _realname(""), _message(""), _server(server)
{	}

User::User(const User& other)
{
	User::operator=(other);
}

User::~User() {	}

User& User::operator=(const User& other)
{
	this->_did_enter = other._did_enter;
	this->_did_register = other._did_register;
	this->_socket = other._socket;
	this->_port = other._port;
	this->_host = other._host;
	this->_nick = other._nick;
	this->_username = other._username;
	this->_realname = other._realname;
	this->_message = other._message;
	this->_server = other._server;

	return *this;
}

int User::port() const
{
	return this->_port;
}

const std::string& User::host() const
{
	return this->_host;
}

const std::string& User::nick() const
{
	return this->_nick;
}

const std::string& User::message() const
{
	return this->_message;
}

void User::append_to_message(const std::string& chunk)
{
	this->_message += chunk;
	if (this->is_message_complete()) {
		// Removing the '\n'
		this->_message.erase(this->_message.size() - 1, 1);

		// The RFC states that every command is delimited by a CRLF (\r\n). Our reference client does not follow this rule
		// To support the RFC, we check if there is a carriage return before the newline, and we remove it
		if (!this->_message.empty() && this->_message[this->_message.size() - 1] == '\r') {
			this->_message.erase(this->_message.size() - 1, 1);
		}
		this->process_message();
	}
}

void User::process_message()
{
	if (this->_message.empty()) {
		return ;
	}
	this->_server->process_message(*this, this->_message);
	this->_message = "";
}

int User::socket() const
{
	return this->_socket;
}

bool User::is_message_complete() const
{
	return	!this->_message.empty()
			&&
			this->_message[this->_message.size() - 1] == '\n';
}

void User::disconnect()
{
	::close(this->_socket);
	this->_socket = -1;
}

}
