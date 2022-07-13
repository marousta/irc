#pragma once

#include <iostream>

#include "Server.hpp"
#include "Replies.hpp"
#include "Errors.hpp"
#include "Helpers.hpp"

#define CLASS_COMMAND(name) \
class name : public Command {								\
	public:													\
		name(ft::Server& serv);								\
															\
		void	execute(std::vector<std::string> args);		\
};


namespace ft {
namespace cmd {

class Command {
	protected:
		ft::Server&		_server;
		std::string		_name;
		std::string		_description;

	public:
		Command(ft::Server& serv, std::string name, std::string desc);
		~Command();

		virtual void	execute(std::vector<std::string> args) = 0;
};

CLASS_COMMAND(Help);
CLASS_COMMAND(Join);
CLASS_COMMAND(Part);
CLASS_COMMAND(Ping);
CLASS_COMMAND(Privmsg);
CLASS_COMMAND(Quit);
CLASS_COMMAND(Who);

}}
