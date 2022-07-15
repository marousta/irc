#include "Commands.hpp"
#include "Server.hpp"

namespace ft {

Command::Command(ft::Server& server, std::string name, std::string desc)
	: _server(server), _name(name), _description(desc)
{	}

Command::~Command()
{	}

}
