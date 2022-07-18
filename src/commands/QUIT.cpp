#include <vector>
#include <iostream>

#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Quit::Quit(ft::Server& server)
	: Command(server, "QUIT", QUIT_DESC)
{	}

void Quit::parse(const std::string& msg)
{
	if (msg.empty() || msg[0] != ':') {
		this->_reason = "Quit: ";
		return ;
	}
	this->_reason = std::string("Quit: ") + &msg[1];
}

void	Quit::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);

	int i = this->_server.find_user_index(*sender);
	if (i >= 0) {
		this->_server.disconnect(i);
	} else {
		std::cerr << "WARNING: User quit but was not connected. This should NOT happen" << std::endl;
	}

	// TODO: Search every channel where the sender is and send a PRIVMSG with this->_reason

	/* TODO:
		The RFC states: 'A client session is terminated with a quit message.  The server
		acknowledges this by sending an ERROR message to the client.'
	*/
}

}}
