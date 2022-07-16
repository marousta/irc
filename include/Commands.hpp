#pragma once

#include <iostream>
#include <vector>

#include "Replies.hpp"
#include "Errors.hpp"
#include "Helpers.hpp"

#define CLASS_COMMAND(name) \
class name : public Command {													\
	public:																		\
		name(ft::Server& serv);													\
																				\
		void	execute(User *sender, const std::vector<std::string>& args);	\
};

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

		virtual void	execute(User *sender, const std::vector<std::string>& args) = 0;
};

namespace cmd {

CLASS_COMMAND(Help);
CLASS_COMMAND(Join);
//CLASS_COMMAND(Mode); /* TODO: change channel modes (actually does only add topic -> optional) */
CLASS_COMMAND(Part);
CLASS_COMMAND(Pass);
CLASS_COMMAND(Ping);
CLASS_COMMAND(Privmsg);
CLASS_COMMAND(Quit);
CLASS_COMMAND(Who);

}}
