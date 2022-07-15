#include "Commands.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

Ping::Ping(ft::Server& server)
	: Command(server, "PING", PING_DESC)
{	}

void	Ping::execute(User *sender, const std::vector<std::string>& args)
{
	sender->send("PONG");
}

}}
