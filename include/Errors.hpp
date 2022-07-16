#pragma once

#include "IRC.hpp"

#define ERR_NOSUCHNICK(nick, to)			std::string(":") + SERVER_NAME + " 401 " + nick + " " + to + " :No such nick or channel name"
#define ERR_NOTEXTTOSEND(nick)				std::string(":") + SERVER_NAME + " 412 " + nick + " :No text to send"
#define ERR_UNKNOWNCOMMAND(cmd)				std::string(":") + SERVER_NAME + " 421 " + cmd + " :Unknown command"
#define ERR_NOLOGIN							std::string(":") + SERVER_NAME + " 444 :User not logged in"
#define ERR_NOTREGISTERED					std::string(":") + SERVER_NAME + " 451 :You have not registered"
#define ERR_PASSWDMISMATCH					std::string(":") + SERVER_NAME + " 464 :Password incorrect"
