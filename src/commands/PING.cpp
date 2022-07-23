#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Ping::Ping(ft::Server& server)
	: Command(server, "PING", PING_DESC)
{	}

void	Ping::parse(const std::string& msg)
{
	(void)msg;
}

void	Ping::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	sender->send("PONG " + msg);
}

}}
