#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Pass::Pass(ft::Server& server)
	: Command(server, "PASS", PART_DESC)
{	}

void Pass::parse(const std::string& msg)
{
	if (::strstr(&msg[0], " ")) {
		throw ERR_NEEDMOREPARAMS(this->_name);
	}

	this->_pass = msg;
}

void	Pass::execute(ft::User *sender, const std::string& msg)
{
	this->parse(msg);

	if (this->_server.check_pass(this->_pass)) {
		sender->entered(true);
	} else {
		throw ERR_PASSWDMISMATCH;
	}

	//TODO:  ERR_ALREADYREGISTERED (462)
}

}}
