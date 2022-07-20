#include <cstring>

#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Pass::Pass(ft::Server& server)
	: Command(server, "PASS", PASS_DESC)
{	}

void	Pass::parse(const std::string& msg)
{
	if (msg.empty()) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	if (msg[0] == ':') {
		this->_pass = &msg[1];
	} else {
		this->_pass = &msg[0];
	}

}

void	Pass::execute(ft::User *sender, const std::string& msg)
{
	if (sender->registered()) {
		throw ERR_ALREADYREGISTERED;
	}

	this->parse(msg);

	if (this->_server.check_pass(this->_pass)) {
		sender->entered(true);
	} else {
		throw ERR_PASSWDMISMATCH;
	}
}

}}
