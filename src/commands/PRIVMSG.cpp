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
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	if (args.size() < 2) {
		throw ERR_NEEDMOREPARAMS("PRIVMSG");
	}

	std::string channel_name = args[0];
	std::string formated_message = ":" + sender->nick() + " " + this->_name + " " + channel_name + " :" + args[1];

	const char *name = &channel_name[0];
	if (name[0] == '#') {
		/* message to channel */
		try {
			Channel *channel = this->_server.get_channel(channel_name);
			if (!channel->user_exist(sender)) {
				throw ERR_NOTONCHANNEL(channel_name);
			}
			channel->dispatch_message(sender, formated_message);
		}
		catch (std::string) {
			throw;
		}
	} else {
		/* message to user */
		ft::User *receiver = this->_server.get_user_nick(channel_name);
		if (!receiver) {
			throw ERR_NOSUCHNICK(sender->nick(), channel_name);
		}
		receiver->send(formated_message);
	}
}

}}
