#include <algorithm>

#include "Errors.hpp"
#include "Colors.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace ft {

Channel::Channel(User *creator, std::string name, Server *server)
	: _name(name), _mode(MODE_DEFAULT), _key(""), _topic(""), _server(server)
{
	this->add_user(creator);
	this->add_operator(creator);
	std::cout << GRN "OK" COLOR_RESET << std::endl;
}

Channel::Channel(User *creator, std::string name, std::string key, Server *server)
	: _name(name), _mode(MODE_K), _key(key), _topic(""), _server(server)
{
	this->add_user(creator);
	this->add_operator(creator);
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
	if (this->_topic.size() > SERVER_TOPICLEN) {
		throw ERR_NEEDMOREPARAMS("TOPIC");
	}
	this->_topic = topic;

	if (this->_topic.empty()) {
		this->unset_mode(MODE_T);
	} else {
		this->mode(MODE_T);
	}
}

void	Channel::add_user(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
	if (it != this->_users.end()) {
		throw ERR_USERONCHANNEL(user->nick, this->_name);
	}
	this->_users.push_back(user);
	this->update_users_list(user);
}

void	Channel::remove_user(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
	if (it != this->_users.end()) {
		this->_users.erase(it);
	}

	if (this->_users.empty()) {
		std::cout << YEL "Removing channel '" COLOR_RESET << this->_name << YEL "'... " COLOR_RESET;
		this->_server->remove_channel(this->_name);
	}
}

bool	Channel::user_exist(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
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
			users += (*it)->nick() + " ";
		}
	}
	return users;
}

void	Channel::add_operator(User *op)
{
	std::vector<User *>::iterator it = this->find_operator(op);
	if (it != this->_operators.end()) {
		throw; /* TODO: error op already operator */
	}
	this->_operators.push_back(op);
}

void	Channel::remove_operator(User *op)
{
	std::vector<User *>::iterator it = this->find_operator(op);
	if (it == this->_operators.end()) {
		throw; /* TODO: error op not found */
	}
	this->_operators.erase(it);
}

bool	Channel::check_operator(User *user) const
{
	std::vector<User *>::const_iterator it = this->find_operator(user);
	if (it == this->_operators.end()) {
		return false;
	}
	return true;
}

void	Channel::dispatch_message(User *sender, std::string message)
{
	for (std::vector<User *>::iterator user = this->_users.begin(); user != this->_users.end(); ++user) {
		if (sender && (*user)->socket() == sender->socket()) {
			continue;
		}
		(*user)->send(message);
	}
}

void	Channel::update_users_list(User *sender) const
{
	for (std::vector<User *>::const_iterator user = this->_users.begin(); user != this->_users.end(); ++user) {
		if (sender->nick() != (*user)->nick()) {
			(*user)->send(RPL_NAMREPLY((*user)->nick(), this->_name, this->list_users()));
			(*user)->send(RPL_ENDOFNAMES((*user)->nick(), this->_name));
		}
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


}
