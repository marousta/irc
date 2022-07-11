#include "net/Client.hpp"

namespace ft {

Client::Client()
	: _socket(TCPSocket())
{	}

Client::Client(const Client& other)
{
	Client::operator=(other);
}

Client::Client(TCPSocket socket, struct sockaddr address, socklen_t addrlen)
	: _socket(socket), _address(address), _addrlen(addrlen)
{	}

Client::~Client()
{	}

Client& Client::operator=(const Client& other)
{
	this->_socket = other._socket;
	this->_address = other._address;
	this->_addrlen = other._addrlen;
	return *this;
}

bool Client::is_valid() const
{
	return this->_socket.is_valid();
}

}
