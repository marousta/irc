#include "Server.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Debug::Debug(ft::Server& server)
	: Command(server, "DEBUG", PART_DESC)
{	}

void	Debug::execute(ft::User *sender, const std::vector<std::string>& args)
{
	(void)args;
	this->_server.print_debug(sender);
}

}}
