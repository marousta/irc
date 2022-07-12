#include "net/TCPSocket.hpp"
#include "net/Client.hpp"
#include <iostream>
#include <fcntl.h>

namespace ft {

TCPSocket::TCPSocket(int fd)
	: _fd(fd)
{	}

TCPSocket::TCPSocket()
	: _fd(-1)
{	}

TCPSocket::TCPSocket(const TCPSocket& other)
{
	TCPSocket::operator=(other);
}

TCPSocket::~TCPSocket()
{	}

void TCPSocket::close()
{
	if (this->is_valid()) {
		::close(this->_fd);
	}
	this->_fd = -1;
}

TCPSocket& TCPSocket::operator=(const TCPSocket& other)
{
	this->_fd = other._fd;
	return *this;
}

bool TCPSocket::is_valid() const
{
	return this->_fd >= 0;
}

Client TCPSocket::accept() const
{
	if (!this->is_valid()) {
		throw std::runtime_error("TCPSocket::accept(): Attempt to call accept() on invalid socket");
	}

	struct sockaddr addr;
	socklen_t addrlen = sizeof(addr);

	TCPSocket socket(::accept(this->_fd, &addr, &addrlen));

	return Client(socket, IPAddress((struct sockaddr_in *)&addr));
}

TCPSocketBuilder::TCPSocketBuilder()
	:	_port(-1)
{	}

TCPSocketBuilder& TCPSocketBuilder::port(uint16_t port)
{
	this->_port = (int32_t)port;
	return *this;
}

TCPSocket TCPSocketBuilder::build() const
{
	if (this->_port < 0) {
		throw std::runtime_error("TCPSocketBuilder::build(): No port provided");
	}

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		throw std::runtime_error("TCPSocketBuilder::build(): Unable to create socket");
	}

	int opt = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
		throw std::runtime_error("TCPSocketBuilder::build(): Could not reuse address");
	}


	struct sockaddr_in address;

	// Since we can't use bzero or memset...
	for (size_t i = 0; i < sizeof(address); ++i) {
		((unsigned char *)(&address))[i] = 0;
	}

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons((uint16_t)this->_port);

	fcntl(fd, F_SETFL, O_NONBLOCK);

	if (::bind(fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
		throw std::runtime_error("TCPSocketBuilder::build(): Bind failed");
	}

	std::cout << "Socket successfully bound" << std::endl;

	if (::listen(fd, 5) != 0) {
		throw std::runtime_error("TCPSocketBuilder::build(): Listening failed");
	}

	std::cout << "Listening on port " << this->_port << std::endl;

	return TCPSocket(fd);
}

}
