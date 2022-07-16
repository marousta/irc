#include "Commands.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

User::User(ft::Server& server)
	: Command(server, "USER", PART_DESC)
{	}

void	User::execute(ft::User *sender, const std::vector<std::string>& args)
{
	try {
		sender->username(args[0], args[3]);
	}
	catch (std::string) {
		throw;
	}
	if (sender->registered()) {
		sender->send(RPL_WELCOME(sender->nick(), sender->username()));
	}
}

}}
