#include "IPAddress.hpp"

namespace ft {

IPAddress::IPAddress()
	: _raw("")
{	}

IPAddress::IPAddress(sockaddr_in *addr)
{
	this->_raw = inet_ntoa(addr->sin_addr);
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
}

const std::string& IPAddress::as_string() const
{
	return this->_raw;
}

}
