#include "Server.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Debug::Debug(ft::Server& server)
	: Command(server, "DEBUG", PART_DESC)
{	}

void Debug::parse(const std::string& msg)
{
	(void)msg;
}

void	Debug::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);
	this->_server.print_debug(sender);
}

}}
