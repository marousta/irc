#pragma once

#include <iostream>
#include <vector>
#include <sstream>

#include "Helpers.hpp"
#include "Replies.hpp"
#include "Errors.hpp"

#define CLASS_COMMAND(name) \
class name : public Command {													\
	public:																		\
		name(ft::Server& serv);													\
																				\
		void	execute(ft::User *sender, const std::vector<std::string>& args);	\
};

/* TODO: OWO WATCH THIS */
template<typename T>
std::string convert_string(T value)
{
	std::stringstream string;

	string << value;

	return string.str();
}

namespace ft {

class Server;
class User;

class Command {
	protected:
		ft::Server&		_server;
		std::string		_name;
		std::string		_description;

	public:
		Command(ft::Server& serv, std::string name, std::string desc);
		virtual ~Command();

		virtual void	execute(ft::User *sender, const std::vector<std::string>& args) = 0;
};

namespace cmd {

void	validate_nick(std::string nick);
void	successfully_registered(ft::User *user, Server *server);

CLASS_COMMAND(Help);
CLASS_COMMAND(Join);
CLASS_COMMAND(Mode);
CLASS_COMMAND(Nick);
CLASS_COMMAND(Part);
CLASS_COMMAND(Pass);
CLASS_COMMAND(Ping);
CLASS_COMMAND(Privmsg);
CLASS_COMMAND(Quit);
CLASS_COMMAND(User);
CLASS_COMMAND(Who);

}}
