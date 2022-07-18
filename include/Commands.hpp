#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

#include "Helpers.hpp"
#include "Replies.hpp"
#include "Errors.hpp"

#define CLASS_COMMAND(name) \
class name : public Command {										\
	public:															\
		name(ft::Server& serv);										\
																	\
		void	execute(ft::User *sender, const std::string& msg);	\
		void	parse(const std::string& msg);						\
};

#define CLASS_COMMAND_BEGIN(name) \
class name : public Command {										\
	public:															\
		name(ft::Server& serv);										\
																	\
		void	execute(ft::User *sender, const std::string& msg);	\
		void	parse(const std::string& msg);						\
	private:

#define CLASS_COMMAND_END() \
};

template<typename T>
std::string convert_string(const T& value)
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

		virtual void	execute(ft::User *sender, const std::string& msg) = 0;

	protected:
		void parse(const std::string& msg);
};

namespace cmd {

void	validate_nick(const std::string& nick);
void	successfully_registered(ft::User *user, Server *server);

CLASS_COMMAND(Debug);

CLASS_COMMAND(Help);

CLASS_COMMAND_BEGIN(Join);
std::vector< std::pair<std::string, std::string> > _channels;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(Mode);
std::vector<std::string> _args;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(Nick);
std::string _nick;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(Part);
std::vector<std::string> _channels;
std::string _reason;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(Pass);
std::string _pass;
CLASS_COMMAND_END();

CLASS_COMMAND(Ping);

CLASS_COMMAND_BEGIN(Privmsg);
std::vector<std::string> _channels;
std::vector<std::string> _users;
std::string _text;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(Quit);
std::string _reason;
CLASS_COMMAND_END();

CLASS_COMMAND_BEGIN(User);
std::string _username;
int _mode;
std::string _realname;
CLASS_COMMAND_END();

}}
