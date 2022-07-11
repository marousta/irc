#include "net/Server.hpp"

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

}
