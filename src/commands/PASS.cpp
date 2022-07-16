#include "Server.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Pass::Pass(ft::Server& server)
	: Command(server, "PASS", PART_DESC)
{	}

void	Pass::execute(ft::User *sender, const std::vector<std::string>& args)
{
	/* TODO; segfault args are empty */
	if (this->_server.check_pass(args[0])) {
		sender->entered(true);
	} else {
		throw ERR_PASSWDMISMATCH;
	}
}

}}
