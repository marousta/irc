#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

void	validate_nick(const std::string& nick)
{
	/* Nicknames are non-empty strings with the following restrictions */
	if (!nick.size()) {
		throw ERR_NONICKNAMEGIVEN;
	}

	if (nick.size() > SERVER_NICKLEN) {
		throw ERR_ERRONEUSNICKNAME(nick);
	}

	/* They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A). */
	/* They MUST NOT start with a character listed as a channel type prefix. */
	if (nick[0] == '$'
	|| nick[0] == ':'
	|| nick[0] == '#') {
		throw ERR_ERRONEUSNICKNAME(nick);
	}

	/* They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40). */
	/* They SHOULD NOT contain any dot character ('.', 0x2E). */
	for (size_t i = 0; i < nick.size(); i++) {
		if (nick[i] == ' '
		|| nick[i] == ','
		|| nick[i] == '*'
		|| nick[i] == '?'
		|| nick[i] == '!'
		|| nick[i] == '@'
		|| nick[i] == '.') {
			throw ERR_ERRONEUSNICKNAME(nick);
		}
	}
}

Nick::Nick(ft::Server& server)
	: Command(server, "NICK", PART_DESC)
{	}

void Nick::parse(const std::string& msg)
{
	this->_nick.clear();
	for (size_t i = 0; i < msg.size(); ++i) {
		if (msg[i] == ' ') {
			throw ERR_NEEDMOREPARAMS(this->_name);
		}
		this->_nick += msg[i];
	}
	validate_nick(this->_nick);
}

void	Nick::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);

	if (this->_server.get_user_nick(this->_nick)) {
		throw ERR_NICKNAMEINUSE(this->_nick);
	}

	sender->nick(this->_nick);
	if (sender->registered()) {
		successfully_registered(sender, &this->_server);
	}
}

}}
