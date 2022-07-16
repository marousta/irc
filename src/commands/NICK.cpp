#include <cstring>

#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

void	validate_nick(std::string nick)
{
	/* Nicknames are non-empty strings with the following restrictions */
	if (!nick.size()) {
		throw ERR_NONICKNAMEGIVEN;
	}

	if (nick.size() > SERVER_NICKLEN) {
		throw ERR_ERRONEUSNICKNAME(nick);
	}

	const char *str = &nick[0];

	/* They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A). */
	/* They MUST NOT start with a character listed as a channel type prefix. */
	if (str[0] == '$'
	|| str[0] == ':'
	|| str[0] == '#') {
		throw ERR_ERRONEUSNICKNAME(nick);
	}

	/* They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40). */
	/* They SHOULD NOT contain any dot character ('.', 0x2E). */
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] == ' '
		|| str[i] == ','
		|| str[i] == '*'
		|| str[i] == '?'
		|| str[i] == '!'
		|| str[i] == '@'
		|| str[i] == '.') {
			throw ERR_ERRONEUSNICKNAME(nick);
		}
	}
}

Nick::Nick(ft::Server& server)
	: Command(server, "NICK", PART_DESC)
{	}

void	Nick::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (!args.size()) {
		throw ERR_NONICKNAMEGIVEN;
	}

	std::string nick = args[0];
	try {
		validate_nick(nick);

		if (this->_server.get_user_nick(nick)) {
			throw ERR_NICKNAMEINUSE(nick);
		}
	}
	catch (std::string) {
		throw;
	}

	sender->nick(nick);
	if (sender->registered()) {
		successfully_registered(sender, &this->_server);
	}
}

}}
