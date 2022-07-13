#include "Commands.hpp"

namespace ft {
namespace cmd {

Part::Part(ft::Server& server)
	: Command(server, "PART", PART_DESC)
{	}

void	Part::execute(std::vector<std::string> args)
{	}

}}
