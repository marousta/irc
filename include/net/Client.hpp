#pragma once

#include "TCPSocket.hpp"

namespace ft {

class Client {
	private:
		TCPSocket _socket;
		struct sockaddr _address;
		socklen_t _addrlen;

	public:
		Client();
		Client(const Client& other);
		Client(TCPSocket socket, struct sockaddr address, socklen_t addrlen);
		~Client();
		Client& operator=(const Client& other);

		bool is_valid() const;
};

}
