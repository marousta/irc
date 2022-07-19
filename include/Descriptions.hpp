#pragma once

#define DEBUG_DESC		std::string("(Custom command) print server related stuff.")
#define HELP_DESC		std::string("[<subject>] Show list of avaliable commands. Show usage of a command if specified.")
#define JOIN_DESC		std::string("<channel>[,<channel>[,...]] [<key>[,<key>[,...]]] Makes the client join the <channel> (comma-separated list), specifying the channel keys (\"passwords\"). A <channel-key> is only needed if the <channel> has the mode \"+k\" set.")
#define MODE_DESC		std::string("<target> [<modestring>] [<mode arguments>] Add, remove, change key/topic of a channel.")
#define NICK_DESC		std::string("<nickname> Change user nickname.")
#define NOTICE_DESC		std::string("<target>{,<target>} <message> Send a <message> to a given <target> and need a delivery report.")
#define PART_DESC		std::string("<target>[,<target>[,...]] [<reason>] Leave given channels.")
#define PASS_DESC		std::string("<password> Used to login in the server.")
#define PING_DESC		std::string("Tests the presence of a connection to a client or server.")
#define PRIVMSG_DESC	std::string("<target>[,<target>[,...]] <message> Send a <message> to a given <target>, which can be a user or a channel.")
#define QUIT_DESC		std::string("<reason> Terminate a user session.")
#define USER_DESC		std::string("<username> <unused> <unused> :<real name> Used to register")
