#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Kick::Kick(ft::Server& server)
	: Command(server, "KICK", KICK_DESC)
{	}

void	Kick::parse(const std::string& msg)
{
	this->_channel.clear();
	this->_users.clear();
	this->_reason.clear();

	const char *mg = &msg[0];
	std::string channel_str;
	while (*mg && *mg != ' ') {
		channel_str += *mg++;
	}

	if (channel_str.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	this->_channel = channel_str;

	while (*mg && *mg == ' ') {
		++mg;
	}

	if (*mg == ':') {
		++mg;
	}

	std::string parsed;
	std::stringstream str(mg);
	while (std::getline(str, parsed, ',')) {
		bool space = false;
		std::string trimed;

		for (size_t i = 0; i < parsed.size(); ++i) {
			if (parsed[i] == ' ') {
				space = true;
				break;
			}
			trimed += parsed[i];
			mg++;
		}
		this->_users.push_back(trimed);
		mg++;
		if (space) break;
	}

	if (this->_users.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
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

void	Kick::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}
	this->parse(msg);

	// PARSING DEBUG
	// std::cout << this->_channel << std::endl;
	// for (std::vector<std::string>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
	// 	std::cout << *it << std::endl;
	// }
	// std::cout << this->_reason << std::endl;

	Channel *channel = this->_server.get_channel_with_name(this->_channel);

	if (!channel->check_operator(sender)) {
		throw ERR_CHANOPRIVSNEEDED(this->_channel);
	}

	std::vector<ft::User *> users;
	for (std::vector<std::string>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		try {
			users.push_back(channel->get_user(*it));
		}
		catch (const std::string&) {
			sender->send(ERR_NOSUCHNICK(sender->nick(), *it));
		}
	}

	for (std::vector<ft::User *>::iterator kicked_user = users.begin(); kicked_user != users.end(); ++kicked_user) {
		channel->dispatch_message(NULL, KICK(sender->nick(), sender->username(), this->_channel, (*kicked_user)->nick(), this->_reason));
	}
}

}}
