#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdint>

namespace ft {

class Client;
class TCPSocket {
	private:
		int _fd;

	public:
		TCPSocket();
		TCPSocket(int fd);

		TCPSocket(const TCPSocket& other);
		~TCPSocket();
		TCPSocket& operator=(const TCPSocket& other);

		bool is_valid() const;

		Client accept() const;

		void close();
};

class TCPSocketBuilder {
	private:
		int32_t _port;

	public:
		TCPSocketBuilder();

		TCPSocketBuilder& port(uint16_t port);

		TCPSocket build() const;
};

}
