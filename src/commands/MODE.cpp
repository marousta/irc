#include "Server.hpp"
#include "Channel.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Mode::Mode(ft::Server& server)
	: Command(server, "MODE", PART_DESC)
{	}

void	Mode::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (args.size() < 1 || args.size() > 3) {
		throw ERR_NEEDMOREPARAMS("MODE");
	}

	try {
		std::string channel_name = args[0];
		Channel *channel = this->_server.get_channel(channel_name);

		const char *arg = &args[1][0];
		if (args.size() == 2) {
			if (arg[0] == '+' || arg[0] != '-') {
				throw ERR_INVALIDMODEPARAM(channel_name, arg, "");
			}
			switch (arg[1])
			{
				case 't': channel->topic(""); break;
				case 'k': channel->key(""); break;
				default: throw ERR_UNKNOWNCOMMAND("MODE");
			}
		} else {
			if (arg[0] == '-' || arg[0] != '+') {
				throw ERR_INVALIDMODEPARAM(channel_name, arg, args[2]);
			}
			switch (arg[1])
			{
				case 't': channel->topic(args[2]); break;
				case 'k': channel->key(args[2]); break;
				default: throw ERR_UNKNOWNCOMMAND("MODE");
			}
		}
	}
	catch (std::string) {
		throw;
	}
}

}}
