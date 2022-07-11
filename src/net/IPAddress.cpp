#include "net/IPAddress.hpp"
#include "arpa/inet.h"
#include <sstream>

namespace ft {

IPAddress::IPAddress()
	: _raw("")
{	}

IPAddress::IPAddress(sockaddr_in *addr)
{
	std::stringstream ss;
	ss << inet_ntoa(addr->sin_addr) << ":" << addr->sin_port;
	this->_raw = ss.str();
}

IPAddress::IPAddress(const IPAddress& other)
{
	IPAddress::operator=(other);
}

IPAddress::~IPAddress()
{	}

IPAddress& IPAddress::operator=(const IPAddress& other)
{
	this->_raw = other._raw;
	return *this;
}

const std::string& IPAddress::as_string() const
{
	return this->_raw;
}

}
