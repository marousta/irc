#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <iostream>
#include "Server.hpp"

namespace ft {

Server::Server(int port)
	: _users(), _pollfds(), _socket(-1)
{
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0) {
		throw std::string("Could not create socket: ") + ::strerror(errno);
	}

	int opt = 1;
	if (::setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw std::string("Could not set socket option 'SO_REUSEADDR': ") + ::strerror(errno);
	}

	if(::fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
		throw std::string("Could not enable non-blocking mode on socket: ") + ::strerror(errno);
	}

	struct sockaddr_in address;

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if(::bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw std::string("Could not bind address to socket: ") + ::strerror(errno);
	}

	if(::listen(this->_socket, 99) < 0) {
		throw std::string("Could not listen on socket: ") + ::strerror(errno);
	}

	std::cout << "IRC Server listening on port " << port << std::endl;
}

Server::~Server()
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		delete this->_users[i];
	}
	::close(this->_socket);
}

void Server::poll()
{
	if (this->_pollfds.empty()) {
		usleep(30000);
	}

	if (::poll(&this->_pollfds[0], this->_pollfds.size(), 30) < 0) {
		throw std::string("Could not poll on socket: ") + ::strerror(errno);
	}

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = ::accept(this->_socket, (struct sockaddr*)&addr, &addrlen);
	if (client_fd >= 0) {
		User *user = new User(client_fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		std::cout << user->host() << ":" << user->port() << " logged in" << std::endl;
		this->_users.push_back(user);
		this->_pollfds.push_back((struct pollfd) {
			.fd = client_fd,
			.events = POLLIN | POLLHUP,
			.revents = 0
		});
	}

	char buf[1024];
	for (size_t i = 0; i < this->_pollfds.size(); ++i) {
		const struct pollfd&	pfd = this->_pollfds[i];
		User					*user = this->_users[i];
		if (pfd.revents & POLLIN) {
			std::memset(buf, 0, 1023);
			int n = recv(pfd.fd, buf, 1023, MSG_PEEK);
			if (n >= 0) {
				if (char *pos = std::strstr(buf, "\n")) {
					size_t len = pos - (char*)buf;
					std::memset(buf, 0, 1023);
					n = recv(pfd.fd, buf, len + 1, 0);
				}
				user->append_to_message(buf);
			} else {
				std::cout << user->host() << ":" << user->port() << " has lost connection (no response)" << std::endl;
				this->disconnect(i);
			}
		}
		if (pfd.revents & POLLHUP) {
			std::cout << user->host() << ":" << user->port() << " has lost connection (connection closed)" << std::endl;
			this->disconnect(i);
		}
	}
}

void Server::disconnect(size_t user_index)
{
	this->_users[user_index]->disconnect();
	delete this->_users[user_index];
	this->_users.erase(this->_users.begin() + user_index);
	this->_pollfds.erase(this->_pollfds.begin() + user_index);
}

}
