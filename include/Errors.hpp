#pragma once

#include "IRC.hpp"

#define ERR_NOSUCHNICK(nick, to)			std::string(":") + SERVER_NAME + " 401 " + nick + " " + nick + " :No such nick or channel name"
#define ERR_NOTEXTTOSEND(nick)				std::string(":") + SERVER_NAME + " 412 " + nick + " :No text to send"
#define ERR_UNKNOWNCOMMAND(cmd)				std::string(":") + SERVER_NAME + " 421 " + cmd + " :Unknown command"
