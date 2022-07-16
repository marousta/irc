#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

Part::Part(ft::Server& server)
	: Command(server, "PART", PART_DESC)
{	}

void	Part::execute(ft::User *sender, const std::vector<std::string>& args)
{
	if (args.size() < 1 || args.size() > 2) {
		throw ERR_NEEDMOREPARAMS("WHO");
	}
}

}}
