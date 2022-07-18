#include "Commands.hpp"

namespace ft {
namespace cmd {

Help::Help(ft::Server& server)
	: Command(server, "HELP", HELP_DESC)
{	}

void Help::parse(const std::string& msg)
{

}

void	Help::execute(ft::User *sender, const std::string& msg)
{	}

}}
