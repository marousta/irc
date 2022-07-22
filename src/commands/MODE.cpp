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

	if (this->_target.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	Channel *channel = this->_server.get_channel_with_name(this->_target);

	if (this->_operation == '+') {
		switch (this->_mode)
		{
			case 'k':	if (!channel->check_operator(sender)) throw ERR_CHANOPRIVSNEEDED(this->_target);
						channel->key(this->_arg);
						return;

			case 'b':	if (this->_arg.empty()) {
							const std::vector<std::string> list = channel->banned_list();

							for (std::vector<std::string>::const_iterator nick = list.begin(); nick != list.end(); ++nick) {
								sender->send(RPL_BANLIST(sender->nick(), this->_target, *nick));
							}
							sender->send(RPL_ENDOFBANLIST(sender->nick(), this->_target));
							return;
						}
						if (!channel->check_operator(sender)) throw ERR_CHANOPRIVSNEEDED(this->_target);
						channel->ban(this->_arg);
						channel->dispatch_message(NULL, MODE_BAN(sender->nick(), this->_target, this->_arg));
						return;

			case 'o':	if (!channel->check_operator(sender)) throw ERR_CHANOPRIVSNEEDED(this->_target);
						channel->op(this->_arg);
						channel->dispatch_message(NULL, MODE_OP(sender->nick(), this->_target, this->_arg));
						return;

			case 't':	channel->mode(MODE_T);
						channel->dispatch_message(NULL, MODE_TOPICPROTECTED(sender->nick(), this->_target));
						return;

			default: throw ERR_INVALIDMODEPARAM(this->_target, this->_operation + this->_mode, "");
		}
	} else if (this->_operation == '-') {
		if (!channel->check_operator(sender)) {
			throw ERR_CHANOPRIVSNEEDED(this->_target);
		}

		switch (this->_mode)
		{
			case 'k':	channel->key("");
						return;

			case 'b':	if (channel->check_banned(this->_arg)) {
							channel->unban(this->_arg);
							channel->dispatch_message(NULL, MODE_UNBAN(sender->nick(), this->_target, this->_arg));
						}
						return;

			case 'o':	channel->deop(this->_arg);
						channel->dispatch_message(NULL, MODE_DEOP(sender->nick(), this->_target, this->_arg));
						return;

			case 't':	channel->unset_mode(MODE_T);
						channel->dispatch_message(NULL, MODE_TOPICUNPROTECTED(sender->nick(), this->_target));
						return;

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
