#include <algorithm>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Commands.hpp"

namespace ft {
namespace cmd {

List::List(ft::Server& server)
	: Command(server, "LIST", LIST_DESC)
{	}

void	List::execute(ft::User *sender, const std::string&)
{
	//TODO: LIST need at least one channel (-> create default channel with read only perms (with witch user?)) in order to update list if no channel have been created
	std::vector<Channel *> channels = this->_server.get_channels();

	sender->send(RPL_LISTSTART(sender->nick()));
	for (std::vector<Channel *>::iterator channel = channels.begin(); channel != channels.end(); ++channel) {
		sender->send(RPL_LIST(sender->nick(), (*channel)->name(), (*channel)->count_users(), (*channel)->topic()));
	}
	sender->send(RPL_LISTEND);
}

}}
