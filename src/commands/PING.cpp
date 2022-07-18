#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Ping::Ping(ft::Server& server)
	: Command(server, "PING", PING_DESC)
{	}

void Ping::parse(const std::string& msg)
{
	(void)msg;
}

void	Ping::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);
	//TODO: Ping is more complex
	//TODO: Check if user is registered ?
	sender->send("PONG");
}

}}
