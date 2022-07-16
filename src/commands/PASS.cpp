#include "Commands.hpp"
#include "Server.hpp"
#include "User.hpp"

namespace ft {
namespace cmd {

Pass::Pass(ft::Server& server)
	: Command(server, "PART", PART_DESC)
{	}

void	Pass::execute(User *sender, const std::vector<std::string>& args)
{
	if (this->_server.check_pass(args[0])) {
		sender->entered(true);
	} else {
		throw ERR_PASSWDMISMATCH;
	}
}

}}
