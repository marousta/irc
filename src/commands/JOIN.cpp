#include "Commands.hpp"

namespace ft {
namespace cmd {

Join::Join(ft::Server& server)
	: Command(server, "JOIN", JOIN_DESC)
{	}

void	Join::execute(User *sender, const std::vector<std::string>& args)
{	}

}}
