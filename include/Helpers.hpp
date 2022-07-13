#pragma once

#define HELP_DESC		""
#define JOIN_DESC		"<channel>[,<channel>[,...]] [<key>[,<key>[,...]]] Makes the client join the <channel> (comma-separated list), specifying the channel keys (\"passwords\"). A <channel-key> is only needed if the <channel> has the mode \"+k\" set."
#define PART_DESC		"<channel>[,<channel>[,...]] [<part-message>] Leave <channel> (comma-separated list), optionally with sending a <part-message> to all the other channel members."
#define PING_DESC		"Tests the presence of a connection to a client or server."
#define PRIVMSG_DESC	"<target>[,<target>[,...]] <message> Send a <message> to a given <target>, which can be a user or a channel."
#define QUIT_DESC		"Terminate a user session."
#define WHO_DESC		"Show a detailed list of users."
