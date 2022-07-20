#include <vector>
#include <iostream>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Quit::Quit(ft::Server& server)
	: Command(server, "QUIT", QUIT_DESC)
{	}

void	Quit::parse(const std::string& msg)
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

	/*
		The RFC states: 'A client session is terminated with a quit message.
		The server acknowledges this by sending an ERROR message to the client.'

		This will never be sended back client because poll is disconnected before
	*/
	sender->send(ERROR("Disconnected"));

	int i = this->_server.find_user_index(*sender);
	if (i >= 0) {
		this->_server.request_disconnect(i);
	} else {
		std::cerr << "WARNING: User quit but was not connected. This should NOT happen" << std::endl;
	}

	std::vector<Channel *> channels = this->_server.get_channels_with_user(sender);
	for (std::vector<Channel *>::iterator channel = channels.begin(); channel != channels.end(); ++channel) {
		(*channel)->dispatch_message(sender, PART(sender->nick(), sender->username(), (*channel)->name(), this->_reason));
	}
}

}}
