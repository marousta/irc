#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Privmsg::Privmsg(ft::Server& server)
	: Command(server, "PRIVMSG", PRIVMSG_DESC)
{	}

void Privmsg::parse(const std::string& msg)
{
	const char *mg = &msg[0];
	std::string targets;

	while (*mg != ' ') {
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
	//TODO:
/*
	std::string channel_name = args[0];
	std::string formated_message = ":" + sender->nick() + " " + this->_name + " " + channel_name + " :" + args[1];

	const char *name = &channel_name[0];
	if (name[0] == '#') {
		// message to channel
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
		// message to user
		ft::User *receiver = this->_server.get_user_nick(channel_name);
		if (!receiver) {
			throw ERR_NOSUCHNICK(sender->nick(), channel_name);
		}
		receiver->send(formated_message);
	}*/
}

}}
