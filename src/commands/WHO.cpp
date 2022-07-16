#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Who::Who(ft::Server& server)
	: Command(server, "WHO", WHO_DESC)
{	}

void	Who::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (!sender->entered()) {
		throw ERR_NOLOGIN;
	}
	if (!sender->registered()) {
		throw ERR_NOTREGISTERED;
	}

	if (args.size() != 2) {
		throw ERR_NEEDMOREPARAMS("WHO");
	}
}

}}
