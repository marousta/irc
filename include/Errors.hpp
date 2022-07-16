#pragma once

#include "IRC.hpp"

#define ERR_NOSUCHNICK(nick, to)				std::string(":") + SERVER_NAME + " 401 " + nick + " " + to + " :No such nick or channel name"
#define ERR_NOSUCHCHANNEL(channel_name)			std::string(":") + SERVER_NAME + " 403 " + channel_name + ":No such channel"
#define ERR_NOTEXTTOSEND(nick)					std::string(":") + SERVER_NAME + " 412 " + nick + " :No text to send"
#define ERR_UNKNOWNCOMMAND(cmd)					std::string(":") + SERVER_NAME + " 421 " + cmd + " :Unknown command"
#define ERR_NONICKNAMEGIVEN						std::string(":") + SERVER_NAME + " 431 :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick)				std::string(":") + SERVER_NAME + " 432 " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick)					std::string(":") + SERVER_NAME + " 433 " + nick + " :Nickname is already in use"
#define ERR_USERONCHANNEL(nick, channel_name)	std::string(":") + SERVER_NAME + " 443 " + channel_name + ":is already on channel"
#define ERR_NOLOGIN								std::string(":") + SERVER_NAME + " 444 :User not logged in"
#define ERR_NOTREGISTERED						std::string(":") + SERVER_NAME + " 451 :You have not registered"
#define ERR_NEEDMOREPARAMS(cmd)					std::string(":") + SERVER_NAME + " 461 " + cmd + " :Not enough parameters"
#define ERR_ALREADYREGISTERED					std::string(":") + SERVER_NAME + " 462 :You may not reregister"
#define ERR_PASSWDMISMATCH						std::string(":") + SERVER_NAME + " 464 :Password incorrect"
#define ERR_BADCHANNELKEY(channel_name)			std::string(":") + SERVER_NAME + " 475 " + channel_name + ":Cannot join channel (+k)"
