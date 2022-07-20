#include "Server.hpp"
#include "Commands.hpp"

namespace ft {

Command::Command(ft::Server& server, std::string name, std::string desc)
	: _server(server), _name(name), _description(desc)
{	}

Command::~Command()
{	}

std::string		Command::name() const
{
	return this->_name;
}

std::string		Command::description() const
{
	return this->_description;
}

void	Command::parse(const std::string&)
{	}

}
