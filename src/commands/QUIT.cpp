#include "Commands.hpp"

namespace ft {
namespace cmd {

Quit::Quit(ft::Server& server)
	: Command(server, "QUIT", QUIT_DESC)
{	}

void	Quit::execute(std::vector<std::string> args)
{	}

}}
