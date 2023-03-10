#include <algorithm>

#include "Errors.hpp"
#include "Colors.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace ft {

Channel::Channel(User *creator, std::string name, Server *server)
	: _name(name), _mode(MODE_DEFAULT | MODE_T), _key(""), _topic(""), _server(server)
{
	this->add_operator(creator);
	this->add_user(creator);
	std::cout << GRN "OK" COLOR_RESET << std::endl;
}

Channel::Channel(User *creator, std::string name, std::string key, Server *server)
	: _name(name), _mode(MODE_K | MODE_T), _key(key), _topic(""), _server(server)
{
	this->add_operator(creator);
	this->add_user(creator);
	std::cout << GRN "OK" COLOR_RESET << std::endl;
}

Channel::~Channel()
{
	std::cout << GRN "OK" COLOR_RESET << std::endl;
}

const std::string& Channel::name() const
{
	return this->_name;
}

short	Channel::mode(void) const
{
	return this->_mode;
}

void	Channel::mode(short mode)
{
	this->_mode |= mode;
}

void	Channel::unset_mode(short mode)
{
	this->_mode &= ~mode;
}

void	Channel::key(std::string key)
{
	this->_key = key;

	if (this->_key.size()) {
		this->mode(MODE_K);
	} else {
		this->unset_mode(MODE_K);
	}
}

bool	Channel::key_compare(std::string key) const
{
	return this->_key == key;
}

const std::string&	Channel::topic(void) const
{
	return this->_topic;
}

void	Channel::topic(std::string topic)
{
	if (topic.size() > SERVER_TOPICLEN) {
		throw ERR_NEEDMOREPARAMS("TOPIC");
	}
	this->_topic = topic;

	if (this->_topic.empty()) {
		this->unset_mode(MODE_T);
	} else {
		this->mode(MODE_T);
	}
}

void	Channel::ban(const std::string& nick)
{
	if (this->check_banned(nick))
		return ;

	this->_ban.push_back(nick);
}

void	Channel::unban(const std::string& nick)
{
	std::vector<std::string>::iterator it = this->find_banned(nick);
	if (it != this->_ban.end()) {
		this->_ban.erase(it);
	}
}

bool	Channel::check_banned(const std::string& nick) const
{
	for (std::vector<std::string>::const_iterator ban_nick = this->_ban.begin(); ban_nick != this->_ban.end(); ++ban_nick) {
		if (*ban_nick == nick) {
			return true;
		}
	}
	return false;
}

const std::vector<std::string>	Channel::banned_list(void) const
{
	return this->_ban;
}

void	Channel::add_user(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
	if (it != this->_users.end()) {
		throw ERR_USERONCHANNEL(user->nick, this->_name);
	}
	if (this->check_banned(user->nick())) {
		throw ERR_BANNEDFROMCHAN(this->_name);
	}

	this->_users.push_back(user);

	std::string nick = user->nick();

	if (this->_mode & MODE_T) {
		user->send(RPL_TOPIC(nick, this->_name, this->_topic));
	} else {
		user->send(RPL_NOTOPIC(nick, this->_name));
	}

	user->send(RPL_NAMREPLY(nick, this->_name, this->list_users()));
	user->send(RPL_ENDOFNAMES(nick, this->_name));

	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		(*it)->send(JOIN(nick, user->username(), this->_name));
	}
	this->update_users();
}

void	Channel::remove_user(User *user)
{
	this->remove_operator(user);

	std::vector<User *>::iterator it = this->find_user(user);
	if (it != this->_users.end()) {
		this->_users.erase(it);
	}

	if (this->_users.empty()) {
		std::cout << YEL "Removing channel '" COLOR_RESET << this->_name << YEL "'... " COLOR_RESET;
		this->_server->remove_channel(this->_name);
	}
}

User*		Channel::get_user(const std::string& nick) const
{
	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->nick() == nick) {
			return *it;
		}
	}
	throw std::string();
}

bool	Channel::user_exist(User *user) const
{
	std::vector<User *>::const_iterator it = this->find_user(user);
	if (it == this->_users.end()) {
		return false;
	}
	return true;
}

const std::string	Channel::list_users(void) const
{
	std::string users;

	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		if ((*it)->registered()) {
			if (this->check_operator((*it))) {
				users += "@" + (*it)->nick() + " ";
			} else {
				users += (*it)->nick() + " ";
			}
		}
	}
	return users;
}

void	Channel::update_users(void) const
{
	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		(*it)->send(RPL_NAMREPLY((*it)->nick(), this->_name, this->list_users()));
		(*it)->send(RPL_ENDOFNAMES((*it)->nick(), this->_name));
	}
}

size_t	Channel::count_users(void) const
{
	return this->_users.size();
}

void	Channel::add_operator(User *op)
{
	std::vector<User *>::iterator it = this->find_operator(op);
	if (it != this->_operators.end()) {
		std::cerr << RED "WARNING: " COLOR_RESET << op->nick() << RED " is already an operator. THIS SHOULD NOT HAPPEN !" COLOR_RESET << std::endl;
		return ;
	}
	this->_operators.push_back(op);
}

void	Channel::remove_operator(User *op)
{
	std::vector<User *>::iterator it = this->find_operator(op);
	if (it != this->_operators.end()) {
		this->_operators.erase(it);
	}
}

bool	Channel::check_operator(User *user) const
{
	std::vector<User *>::const_iterator it = this->find_operator(user);
	if (it == this->_operators.end()) {
		return false;
	}
	return true;
}

void	Channel::op(const std::string& nick)
{
	User *user = this->get_user(nick);

	if (this->check_operator(user)) {
		return ;
	}
	this->add_operator(user);
}

void	Channel::deop(const std::string& nick)
{
	User *user = this->get_user(nick);

	if (this->check_operator(user)) {
		this->remove_operator(user);
	}
}

void	Channel::dispatch_message(User *sender, const std::string& message) const
{
	for (std::vector<User *>::const_iterator user = this->_users.begin(); user != this->_users.end(); ++user) {
		if (sender && (*user)->socket() == sender->socket()) {
			continue;
		}
		(*user)->send(message);
	}
}

std::vector<User *>::iterator		Channel::find_user(User *user)
{
	return std::find(this->_users.begin(), this->_users.end(), user);
}

std::vector<User *>::const_iterator	Channel::find_user(User *user) const
{
	return std::find(this->_users.begin(), this->_users.end(), user);
}

std::vector<User *>::iterator		Channel::find_operator(User *op)
{
	return std::find(this->_operators.begin(), this->_operators.end(), op);
}

std::vector<User *>::const_iterator	Channel::find_operator(User *op) const
{
	return std::find(this->_operators.begin(), this->_operators.end(), op);
}

std::vector<std::string>::iterator		Channel::find_banned(const std::string& nick)
{
	return std::find(this->_ban.begin(), this->_ban.end(), nick);
}

std::vector<std::string>::const_iterator	Channel::find_banned(const std::string& nick) const
{
	return std::find(this->_ban.begin(), this->_ban.end(), nick);
}

}
