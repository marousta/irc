#include <cstdlib>
#include <cstring>

#include "Colors.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

void	successfully_registered(ft::User *user, Server *server)
{
	std::string nick = user->nick();

	user->send(RPL_WELCOME(nick, user->username()));
	user->send(RPL_YOURHOST(nick));
	user->send(RPL_MYINFO(nick));
	user->send(RPL_BOUNCE(nick));
	user->send(RPL_LUSERCLIENT(nick, convert_string(server->user_count())));
	user->send(RPL_LUSERCHANNELS(nick, convert_string(server->channel_count())));
}

User::User(ft::Server& server)
	: Command(server, "USER", USER_DESC)
{	}

void	User::parse(const std::string& msg)
{
	std::vector<std::string> parts;

	const char *mg = &msg[0];
	std::string current;
	while (*mg && parts.size() != 3) {
		if (*mg == ' ') {
			if (current.empty()) {
				++mg;
				continue ;
			}
			parts.push_back(current);
			current.clear();
			++mg;
			continue ;
		}
		current += *mg++;
	}

	if (parts.size() != 3) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	for (size_t i = 0; i < 3; ++i) {
		if (::strstr(&parts[i][0], ":")) {
			throw ERR_NEEDMOREPARAMS(this->_name);
		}
	}

	while (*mg == ' ') {
		++mg;
	}

	if (*mg == ':') {
		++mg;
	}

	while (*mg) {
		if (*mg == ':') {
			throw ERR_NEEDMOREPARAMS(this->_name);
		}
		++mg;
	}

	for (size_t i = 0; i < parts[1].size(); ++i) {
		if (!::isdigit(parts[1][i])) {
			parts[1] = "0";
			break ;
		}
	}

	this->_realname = mg;
	this->_username = parts[0];
	this->_mode = ::atoi(&parts[1][0]);
}

void	User::execute(ft::User *sender, const std::string& msg)
{
	if (sender->registered()) {
		throw ERR_ALREADYREGISTERED;
	}

	this->parse(msg);

	sender->username(this->_username);
	sender->realname(this->_realname);

	if (!sender->registered() && sender->check_register()) {
		successfully_registered(sender, &this->_server);
	}
}

}}
