#include <algorithm>

#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Help::Help(ft::Server& server)
	: Command(server, "HELP", HELP_DESC)
{	}

void	Help::parse(const std::string& msg)
{
	this->_cmd.clear();

	const char *mg = &msg[0];
	while (*mg && *mg != ' ') {
		this->_cmd += *mg++;
	}
}

void	Help::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);

	std::map<std::string, Command *> commands = this->_server.get_commands();

	if (this->_cmd.empty()) {
		sender->send(HELP(sender->nick(), sender->nick(), "HELP [<command>]"));
		sender->send(HELP(sender->nick(), sender->nick(), "List of all available commands:"));
		for (std::map<std::string, Command *>::iterator cmd = commands.begin(); cmd != commands.end(); ++cmd) {
			if (cmd->first == "DEBUG") {
				continue;
			}
			sender->send(HELP(sender->nick(), sender->nick(), cmd->second->name()));
		}
	} else {
		/* Put command in uppercase, just in case */
		std::transform(this->_cmd.begin(), this->_cmd.end(), this->_cmd.begin(), ::toupper);

		if (this->_cmd == "DEBUG") {
			throw ERR_UNKNOWNCOMMAND(this->_cmd);
		}

		try {
			ft::Command *cmd = commands.at(this->_cmd);
			sender->send(HELP(sender->nick(), sender->nick(), cmd->name() + ": " + cmd->description()));
		}
		catch (const std::exception&) {
			throw ERR_UNKNOWNCOMMAND(this->_cmd);
		}

	}
}

}}
