#pragma once

#include "TCPSocket.hpp"
#include "IPAddress.hpp"

namespace ft {

class Client {
	private:
		TCPSocket 	_socket;
		IPAddress 	_address;

	public:
		Client();
		Client(const Client& other);
		Client(TCPSocket socket, IPAddress address);
		~Client();
		Client& operator=(const Client& other);

		bool is_valid() const;
		TCPSocket	&socket();
		const IPAddress& address() const;
};

}
