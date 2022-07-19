#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Mode::Mode(ft::Server& server)
	: Command(server, "MODE", MODE_DESC)
{	}

void	Mode::parse(const std::string& msg)
{
	this->_target.clear();
	this->_mode = 0;
	this->_operation = '\0';
	this->_arg.clear();

	const char *mg = &msg[0];

	while (*mg && *mg != ' ') {
		this->_target += *mg++;
	}
	while (*mg == ' ') {
		++mg;
	}
	if (!*mg) {
		return ;
	}

	std::string modestring;
	while (*mg && *mg != ' ') {
		modestring += *mg++;
	}

	if (modestring.size() != 2 || (modestring[0] != '+' && modestring[0] != '-')) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	this->_operation = modestring[0];
	this->_mode = modestring[1];

	while (*mg == ' ') {
		++mg;
	}

	if (*mg == ':') {
		++mg;
	}

	this->_arg = mg;
}

void	Mode::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	if (this->_target.empty()
	|| (this->_operation == '+' && this->_arg.empty())) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	Channel *channel = this->_server.get_channel(this->_target);

	if (this->_operation == '+') {
		switch (this->_mode)
		{
			case 't': channel->topic(this->_arg); break;
			case 'k': channel->key(this->_arg); break;
			default: throw ERR_INVALIDMODEPARAM(this->_target, this->_operation + this->_mode, "");
		}
	} else if (this->_operation == '-') {
		switch (this->_mode)
		{
			case 't': channel->topic(""); break;
			case 'k': channel->key(""); break;
			default: throw ERR_INVALIDMODEPARAM(this->_target, this->_operation + this->_mode, "");
		}
	} else {
		std::string ret("+");

		short mode = channel->mode();
		if (mode & MODE_K)			ret += "k";
		if (mode & MODE_T)			ret += "t";

		sender->send("MODE " + this->_target + " " + ret);
	}
}

}}
