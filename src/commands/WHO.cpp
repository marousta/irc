#include "Commands.hpp"

namespace ft {
namespace cmd {

Who::Who(ft::Server& server)
	: Command(server, "WHO", WHO_DESC)
{	}

void	Who::execute(std::vector<std::string> args)
{	}

}}
