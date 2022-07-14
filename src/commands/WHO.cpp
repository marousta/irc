#include "Commands.hpp"

namespace ft {
namespace cmd {

Who::Who(ft::Server& server)
	: Command(server, "WHO", WHO_DESC)
{	}

void	Who::execute(User *sender, const std::vector<std::string>& args)
{	}

}}
