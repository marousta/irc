#include "net/Client.hpp"
#include <iostream>

namespace ft {

Client::Client()
	: _socket(TCPSocket())
{	}

Client::Client(const Client& other)
{
	Client::operator=(other);
}

Client::Client(TCPSocket socket, IPAddress address)
	: _socket(socket), _address(address)
{	}

Client::~Client()
{	}

Client& Client::operator=(const Client& other)
{
	this->_socket = other._socket;
	this->_address = other._address;
	return *this;
}

bool Client::is_valid() const
{
	return this->_socket.is_valid();
}

const IPAddress& Client::address() const
{
	return this->_address;
}

}
