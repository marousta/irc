#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Join::Join(ft::Server& server)
	: Command(server, "JOIN", JOIN_DESC)
{	}

void	Join::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	if (args.size() < 1 || args.size() > 2) {
		throw ERR_NEEDMOREPARAMS("JOIN");
	}

	try {
		this->_server.join_channel(sender, args);
	}
	catch (std::exception &e) {
		throw; /* TODO: error something when wrong */
	}
}

}}
