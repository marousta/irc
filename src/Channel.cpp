#include "Channel.hpp"

namespace ft {

Channel::Channel(User *creator, std::string name)
	: _name(name)
{
	this->set_mode(MODE_DEFAULT);
	this->_key = "";
	this->set_topic("");
	this->add_user(creator);
	this->add_operator(creator);
	std::cout << "Channel " << name << " created." << std::endl;
}

Channel::Channel(User *creator, std::string name, std::string key)
	: _name(name), _key(key)
{
	this->set_mode(MODE_K);
	this->set_topic("");
	this->add_user(creator);
	this->add_operator(creator);
	std::cout << "Channel " << name << " created with key '" << key << "'." << std::endl;
}

Channel::~Channel()
{

}

void	Channel::set_mode(uint32_t mode)
{
	this->_mode |= mode;
}

void	Channel::unset_mode(uint32_t mode)
{
	this->_mode ^= mode;
}

void	Channel::set_topic(std::string topic)
{
	this->_topic = topic;
}

void	Channel::add_user(User *user)
{
	this->_users.push_back(user);
}

void	Channel::remove_user(User *user)
{

}

void	Channel::add_operator(User *user)
{
	this->_users.push_back(user);
}

void	Channel::remove_operator(User *user)
{

}

void	dispatch_message(User *sender, std::string message)
{

}

}
