#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Privmsg::Privmsg(ft::Server& server)
	: Command(server, "PRIVMSG", PRIVMSG_DESC)
{	}

void	Privmsg::execute(ft::User *sender, const std::vector<std::string>& args)
{
	/* TODO: Check arguments */
	if (args.size() < 2) {
		throw ERR_NEEDMOREPARAMS("PRIVMSG");
	}

	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	std::string formated_message = ":" + sender->nick() + " " + this->_name + " " + args[0] + " :" + args[1];

	const char *name = &args[0][0];
	if (name[0] == '#') {
		/* message to channel */
		try {
			Channel *channel = this->_server.get_channel(args[0]);
			channel->dispatch_message(sender, formated_message);
		}
		catch (std::string) {
			throw;
		}
	} else {
		/* message to user */
		ft::User *receiver = this->_server.get_user_nick(args[0]);
		if (!receiver) {
			throw ERR_NOSUCHNICK(sender->nick(), args[0]);
		}
		receiver->send(formated_message);
	}
}

}}
