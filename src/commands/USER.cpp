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
	user->send(RPL_CREATED(nick));
	user->send(RPL_MYINFO(nick));
	user->send(RPL_BOUNCE(nick));
	user->send(RPL_LUSERCLIENT(nick, convert_string(server->user_count())));
	user->send(RPL_LUSERCHANNELS(nick, convert_string(server->channel_count())));
}

User::User(ft::Server& server)
	: Command(server, "USER", PART_DESC)
{	}

void	User::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (args.size() < 4) {
		throw ERR_NEEDMOREPARAMS("USER");
	}

	try {
		sender->username(args[0], args[3]);
	}
	catch (std::string) {
		throw;
	}
	if (sender->registered()) {
		successfully_registered(sender, &this->_server);
	}
}

}}
