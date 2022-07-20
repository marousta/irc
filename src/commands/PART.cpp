#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Part::Part(ft::Server& server)
	: Command(server, "PART", PART_DESC)
{	}

void	Part::parse(const std::string& msg)
{
	this->_channels.clear();
	this->_reason.clear();

	std::string parsed;

	const char *mg = &msg[0];
	std::string channel_str;

	while (*mg && *mg != ' ') {
		channel_str += *mg++;
	}

	if (channel_str.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	std::stringstream str(channel_str);
	while (std::getline(str, parsed, ',')) {
		this->_channels.push_back(parsed);
	}

	while (*mg && *mg == ' ') {
		++mg;
	}

	if (*mg == ':') {
		++mg;
	}

	if (*mg) {
		this->_reason = mg;
	}
}

void	Part::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	for (size_t i = 0; i < this->_channels.size(); ++i) {
		Channel *channel;
		try {
			channel = this->_server.get_channel_with_name(this->_channels[i]);
		} catch (const std::string& err) {
			sender->send(err);
			continue ;
		}
		if (!channel->user_exist(sender)) {
			sender->send(ERR_NOTONCHANNEL(channel->name()));
			continue ;
		} else {
			channel->dispatch_message(NULL, PART(sender->nick(), sender->username(), channel->name(), this->_reason));
			channel->remove_user(sender);
		}
	}
}

}}
