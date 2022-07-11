#pragma once

#include <string>
#include "TCPSocket.hpp"

namespace ft {

class IPAddress {
	private:
		std::string _raw;

	public:
		IPAddress();
		IPAddress(sockaddr_in *addr);
		IPAddress(const IPAddress& other);
		~IPAddress();
		IPAddress& operator=(const IPAddress& other);

		const std::string& as_string() const;
};

}
