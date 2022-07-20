#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Topic::Topic(ft::Server& server)
	: Command(server, "TOPIC", TOPIC_DESC)
{	}

void	Topic::parse(const std::string& msg)
{
	this->_channel.clear();
	this->_topic.clear();

	const char *mg = &msg[0];
	while (*mg) {
		if (*mg == ' ') {
			break;
		}
		this->_channel += *mg;
		++mg;
	}

	while (*mg == ' ') {
		++mg;
	}

	if (*mg == ':') {
		++mg;
	}

	this->_topic = mg;

	if (this->_channel.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}
}

void	Topic::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	Channel *channel = this->_server.get_channel(this->_channel);

	if (this->_topic.empty()) {
		sender->send(RPL_TOPIC(sender->nick(), this->_channel, channel->topic()));
	} else {
		channel->topic(this->_topic);
		channel->dispatch_message(NULL, TOPIC(sender->nick(), sender->username(), this->_channel, this->_topic));
	}
}

}}
