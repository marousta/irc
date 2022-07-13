#include "Commands.hpp"

namespace ft {
namespace cmd {

Ping::Ping(ft::Server& server)
	: Command(server, "PING", PING_DESC)
{	}

void	Ping::execute(std::vector<std::string> args)
{	}

}}
