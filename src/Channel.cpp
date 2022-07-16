#include <algorithm>

#include "Errors.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace ft {

Channel::Channel(User *creator, std::string name)
	: _name(this->format_name(name)), _mode(MODE_DEFAULT), _key(""), _topic("")
{
	this->add_user(creator);
	this->add_operator(creator);
	std::cout << "Channel " << this->_name << " created." << std::endl;
}

Channel::Channel(User *creator, std::string name, std::string key)
	: _name(this->format_name(name)), _mode(MODE_K), _key(key), _topic("")
{
	this->add_user(creator);
	this->add_operator(creator);
	std::cout << "Channel " << this->_name << " created with key '" << key << "'." << std::endl;
}

Channel::~Channel()
{

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

bool Channel::key_compare(std::string key) const
{
	return this->_key == key;
}

const std::string&	Channel::topic(void) const
{
	return this->_topic;
}

void	Channel::topic(std::string topic)
{
	this->_topic = topic;
}

void	Channel::add_user(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
	if (it != this->_users.end()) {
		throw ERR_USERONCHANNEL(user->nick, this->_name);
	}
	this->_users.push_back(user);
}

void	Channel::remove_user(User *user)
{
	std::vector<User *>::iterator it = this->find_user(user);
	if (it == this->_users.end()) {
		throw; /* TODO: error user not found */
	}
	this->_users.erase(it);
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

void	Channel::dispatch_message(User *sender, std::string message)
{
	for (std::vector<User *>::iterator user = this->_users.begin(); user != this->_users.end(); ++user) {
		if ((*user)->socket() == sender->socket()) {
			continue;
		}
		(*user)->send(message);
	}
}

std::string	Channel::format_name(std::string name)
{
	const char *n = &name[0];
	if (n[0] != '#') {
		return std::string("#") + name;
	}
	return name;
}

std::vector<User *>::iterator Channel::find_user(User *user)
{
	return std::find(this->_users.begin(), this->_users.end(), user);
}

std::vector<User *>::iterator Channel::find_operator(User *op)
{
	return std::find(this->_operators.begin(), this->_operators.end(), op);
}

}
