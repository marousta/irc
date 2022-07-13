#pragma once

#include "IRC.hpp"

#define ERR_NOSUCHNICK(nick, to)			":" + SERVER_NAME + " 401 " + nick + " " + nick + " :No such nick or channel name"
#define ERR_NOTEXTTOSEND(nick)				":" + SERVER_NAME + " 412 " + nick + " :No text to send"
