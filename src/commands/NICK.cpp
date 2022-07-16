#include "Commands.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

Nick::Nick(ft::Server& server)
	: Command(server, "NICK", PART_DESC)
{	}

void	Nick::execute(ft::User *sender, const std::vector<std::string>& args)
{
	/* TODO: check nick */
	/*
		Nicknames are non-empty strings with the following restrictions:

		They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
		They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
		They MUST NOT start with a character listed as a channel type prefix.
		They SHOULD NOT contain any dot character ('.', 0x2E).
	*/
	sender->nick(args[0]);
	if (sender->registered()) {
		sender->send(RPL_WELCOME(sender->nick(), sender->username()));
	}
}

}}
