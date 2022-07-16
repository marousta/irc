#include "Commands.hpp"

namespace ft {
namespace cmd {

Help::Help(ft::Server& server)
	: Command(server, "HELP", HELP_DESC)
{	}

void	Help::execute(ft::User *sender, const std::vector<std::string>& args)
{	}

}}
