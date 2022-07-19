#include <sstream>

#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Join::Join(ft::Server& server)
	: Command(server, "JOIN", JOIN_DESC)
{	}

void	Join::parse(const std::string& msg)
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
	std::vector<std::string> channels;
	while (std::getline(channel_str, parsed, ',')) {
		channels.push_back(parsed);
	}

	std::vector<std::string> keys;
	if (parts.size() == 2) {
		std::stringstream keys_str(parts[1]);
		while (std::getline(keys_str, parsed, ',')) {
			keys.push_back(parsed);
		}
	}

	this->_channels.clear();
	for (size_t i = 0; i < channels.size(); ++i) {
		this->_channels.push_back(std::make_pair(channels[i], (i < keys.size()) ? keys[i] : ""));
	}
}

void	Join::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	for (size_t i = 0; i < this->_channels.size(); ++i) {
		if (this->_channels[i].first[0] != '#' || this->_channels[i].first.size() > 50) {
			sender->send(ERR_NOSUCHCHANNEL(this->_channels[i].first));
		} else {
			try {
				this->_server.join_channel(sender, this->_channels[i].first, this->_channels[i].second);
			}
			catch (std::string& e) {
				sender->send(e);
			}
		}
	}
}

}}
