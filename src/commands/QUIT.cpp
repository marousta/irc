#include <vector>
#include <iostream>

#include "Commands.hpp"
#include "Server.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

Quit::Quit(ft::Server& server)
	: Command(server, "QUIT", QUIT_DESC)
{	}

void	Quit::execute(User *sender, const std::vector<std::string>& args) {
	if (args.size() != 1) {
		std::cerr << "WARNING: 'QUIT' should have 1 argument" << std::endl;
	}
	int i = this->_server.find_user_index(*sender);
	if (i >= 0) {
		this->_server.disconnect(i);
	} else {
		std::cerr << "WARNING: User quit but was not connected. This should NOT happen" << std::endl;
	}

	/* TODO:
		The RFC states: 'A client session is terminated with a quit message.  The server
		acknowledges this by sending an ERROR message to the client.'
	*/
}

}}
