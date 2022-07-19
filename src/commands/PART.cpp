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
	std::string parsed;
	std::stringstream str(msg);

	std::vector<std::string> parts;

	while (std::getline(str, parsed, ' ')) {
		parts.push_back(parsed);
	}

	if (parts.empty() || (parts.size() > 2)) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	std::stringstream channel_str(parts[0]);

	this->_channels.clear();
	while (std::getline(channel_str, parsed, ',')) {
		this->_channels.push_back(parsed);
	}

	if (parts.size() == 2) {
		this->_reason = parts[1];
	} else {
		this->_reason.clear();
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
			channel = this->_server.get_channel(this->_channels[i]);
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
