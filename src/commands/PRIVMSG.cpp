#include <cstring>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Privmsg::Privmsg(ft::Server& server)
	: Command(server, "PRIVMSG", PRIVMSG_DESC)
{	}

void	Privmsg::parse(const std::string& msg)
{
	this->_channels.clear();
	this->_users.clear();
	this->_text.clear();

	const char *mg = &msg[0];

	std::string first;
	while (*mg && *mg != ' ') {
		first += *mg++;
	}
	while (*mg == ' ') {
		++mg;
	}

	if (first.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	if (*mg == ':') {
		++mg;
	}

	std::string parsed;
	std::stringstream str(first);

	while (std::getline(str, parsed, ',')) {
		if (parsed[0] == '#') {
			this->_channels.push_back(parsed);
			continue ;
		}
		this->_users.push_back(parsed);
	}

	this->_text = mg;
}

void	Privmsg::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	if (this->_text.empty()
	|| (this->_channels.empty() && this->_users.empty())) {
		throw ERR_NOTEXTTOSEND(sender->nick());
	}

	for (std::vector<std::string>::iterator channel_name = this->_channels.begin(); channel_name != this->_channels.end(); ++channel_name) {
		try {
			Channel *channel = this->_server.get_channel_with_name(*channel_name);
			channel->dispatch_message(sender, PRIVMSG(sender->nick(), sender->username(), *channel_name, this->_text));
		}
		catch (std::string& e) {
			sender->send(e);
		}
	}

	for (std::vector<std::string>::iterator receiver_nick = this->_users.begin(); receiver_nick != this->_users.end(); ++receiver_nick) {
		try {
			ft::User *receiver = this->_server.get_user_nick(*receiver_nick);

			if (!receiver) {
				throw ERR_NOSUCHNICK(sender->nick(), *receiver_nick);
			}
			receiver->send(PRIVMSG(sender->nick(), sender->username(), *receiver_nick, this->_text));
		}
		catch (std::string& e) {
			sender->send(e);
		}
	}
}

}}
