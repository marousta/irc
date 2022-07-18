#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Mode::Mode(ft::Server& server)
	: Command(server, "MODE", PART_DESC)
{	}

void Mode::parse(const std::string& msg)
{
	this->_args.clear();
	std::stringstream str(msg);
	std::string parsed;

	while (std::getline(str, parsed, ' ')) {
		this->_args.push_back(parsed);
	}
}

void	Mode::execute(ft::User *sender, const std::string& msg)
{
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	this->parse(msg);

	if (this->_args.size() < 1 || this->_args.size() > 3) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	std::string channel_name = this->_args[0];
	Channel *channel = this->_server.get_channel(channel_name);

	if (!channel->check_operator(sender)) {
		throw ERR_CHANOPRIVSNEEDED(channel_name);
	}

	char *arg = &this->_args[0][0];
	if (this->_args.size() == 1) {
		/* remove mode only - and not params is accepted */
		if (arg[0] != '#') {
			throw ERR_INVALIDMODEPARAM(channel_name, arg, "");
		}

		short mode = channel->mode();
		std::string ret("+");

		if (mode & MODE_DEFAULT) {
			ret += "b";
		}
		if (mode & MODE_K) {
			ret += "k";
		}
		if (mode & MODE_T) {
			ret += "t";
		}

		sender->send("MODE " + channel_name + " " + ret);

		return ;
	}

	arg = &this->_args[1][0];
	if (this->_args.size() == 2) {
		/* remove mode only - and not params is accepted */
		if (arg[0] == '+' || arg[0] != '-') {
			throw ERR_INVALIDMODEPARAM(channel_name, arg, "");
		}
		switch (arg[1])
		{
			case 't': channel->topic(""); break;
			case 'k': channel->key(""); break;
			default: throw ERR_UNKNOWNCOMMAND("MODE");
		}
		return ;
	}
	if (this->_args.size() == 3) {
		/* add mode only + and one param is needed */
		if (arg[0] == '-' || arg[0] != '+') {
			throw ERR_INVALIDMODEPARAM(channel_name, arg, this->_args[2]);
		}
		switch (arg[1])
		{
			case 't': channel->topic(this->_args[2]); break;
			case 'k': channel->key(this->_args[2]); break;
			default: throw ERR_UNKNOWNCOMMAND("MODE");
		}
		return ;
	}
}

}}
