#include "Server.hpp"
#include "Commands.hpp"

namespace ft {

Command::Command(ft::Server& server, std::string name, std::string desc)
	: _server(server), _name(name), _description(desc)
{	}

Command::~Command()
{	}

void Command::parse(const std::string& msg)
{	}

}
