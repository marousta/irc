#include "Commands.hpp"

namespace ft {
namespace cmd {

Join::Join(ft::Server& server)
	: Command(server, "JOIN", JOIN_DESC)
{	}

void	Join::execute(std::vector<std::string> args)
{	}

}}
