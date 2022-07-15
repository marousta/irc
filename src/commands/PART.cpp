#include "Commands.hpp"

namespace ft {
namespace cmd {

Part::Part(ft::Server& server)
	: Command(server, "PART", PART_DESC)
{	}

void	Part::execute(User *sender, const std::vector<std::string>& args)
{

}

}}
