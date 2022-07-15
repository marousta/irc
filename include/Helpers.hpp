#pragma once

#define HELP_DESC		std::string("")
#define JOIN_DESC		std::string("<channel>[,<channel>[,...]] [<key>[,<key>[,...]]] Makes the client join the <channel> (comma-separated list), specifying the channel keys (\"passwords\"). A <channel-key> is only needed if the <channel> has the mode \"+k\" set.")
#define PART_DESC		std::string("<channel>[,<channel>[,...]] [<part-message>] Leave <channel> (comma-separated list), optionally with sending a <part-message> to all the other channel members.")
#define PING_DESC		std::string("Tests the presence of a connection to a client or server.")
#define PRIVMSG_DESC	std::string("<target>[,<target>[,...]] <message> Send a <message> to a given <target>, which can be a user or a channel.")
#define QUIT_DESC		std::string("Terminate a user session.")
#define WHO_DESC		std::string("Show a detailed list of users.")
