#include "net/Server.hpp"

#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace ft {

Server::Server()
	: _socket(TCPSocket())
{	}

Server::Server(TCPSocket socket)
	: _socket(socket)
{	}

Server::Server(const Server& server)
{
	Server::operator=(server);
}

Server::~Server()
{
	for (size_t i = 0; i < this->_clients.size(); ++i) {
		delete _clients[i];
	}
}

Server& Server::operator=(const Server& other)
{
	this->_socket = other._socket;
	return *this;
}

TCPSocket Server::socket()
{
	return this->_socket;
}

void Server::close()
{
	this->_socket.close();
}

void Server::handle_new_connections()
{
	ft::Client client;
	do {
		client = this->socket().accept();
		if (client.is_valid()) {
			this->_clients.push_back(new Client(client));
			std::cout << "Client connected with address " << client.address().as_string() << std::endl;
		}
	} while (client.is_valid());
}

void Server::poll()
{
	std::vector<struct pollfd> pool;

	for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		struct pollfd pfd = {
			.fd = (*it)->socket().raw(),
			.events = POLLIN | POLLHUP,
			.revents = 0
		};
		pool.push_back(pfd);
	}

	::poll(&pool[0], pool.size(), 15);

	for (size_t i = 0; i < pool.size(); ++i) {
		if (pool[i].revents & POLLHUP) {
			std::cout << this->_clients[i]->address().as_string() << " as disconnected" << std::endl;
			this->_clients.erase(this->_clients.begin() + i);
			pool.erase(pool.begin() + i);
			--i;
			continue ;
		} else if (pool[i].revents & POLLIN) {
			std::cout << this->_clients[i]->address().as_string() << " has data to send" << std::endl;
			std::string packet;
			char *buf = new char[1024];
			buf[0] = 0;
			for (;;) {
				ssize_t stat = ::recv(pool[i].fd, buf, 1024, 0);
				if (stat == 0) {
					break ;
				} else if (stat == -1) {
					std::cerr << "Error reading data from " << this->_clients[i]->address().as_string() << std::endl;
					perror("recv");
					exit(-1);
				} else {
					for (ssize_t j = 0; j < stat; ++j) {
						packet += buf[i];
					}
				}
			}
			delete[] buf;
			if (!packet.empty()) {
				std::cout << "Message: " << packet << std::endl;
			}
		}
	}
}

}
