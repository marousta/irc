#include "Commands.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

Privmsg::Privmsg(ft::Server& server)
	: Command(server, "PRIVMSG", PRIVMSG_DESC)
{	}

void	Privmsg::execute(User *sender, const std::vector<std::string>& args)
{
	std::string formated_message = ":" + sender->nick() + " " + this->_name + " " + args[0] + " :" + args[1];

	const char *name = &args[0][0];
	if (name[0] == '#') {
		/* message to channel */
		try {
			Channel *channel = this->_server.get_channel(args[0]);
			channel->dispatch_message(sender, formated_message);
		}
		catch (std::exception& e) {
			(void)e;
			throw; /* TODO: error no channel found */
		}
	} else {
		/* message to user */
		try {
			User *receiver = this->_server.get_user(args[0]);
			receiver->send(formated_message);
		}
		catch (std::exception& e) {
			(void)e;
			throw; /* TODO: error user not found */
		}
	}
}

}}
