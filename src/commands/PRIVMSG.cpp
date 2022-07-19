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
	const char *mg = &msg[0];
	std::string targets;

	while (*mg && *mg != ' ') {
		targets += *mg++;
	}
	if (!*mg) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	std::string parsed;
	std::stringstream str(targets);

	this->_channels.clear();
	this->_users.clear();
	while (std::getline(str, parsed, ',')) {
		if (parsed[0] == '#') {
			this->_channels.push_back(parsed);
		} else {
			this->_users.push_back(parsed);
		}
	}

	while (*mg == ' ') {
		++mg;
	}

	if (*mg != ':') {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}
	++mg;

	if (::strstr(mg, ":")) {
		throw ERR_NEEDMOREPARAMS(this->_name);
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

	for (std::vector<std::string>::iterator channel_name = this->_channels.begin(); channel_name != this->_channels.end(); ++channel_name) {
		try {
			Channel *channel = this->_server.get_channel(*channel_name);
			channel->dispatch_message(sender, PRIVMSG(sender->nick(), *channel_name, this->_text));
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
			receiver->send(PRIVMSG(sender->nick(), *receiver_nick, this->_text));
		}
		catch (std::string& e) {
			sender->send(e);
		}
	}
}

}}
