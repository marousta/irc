001    RPL_WELCOME		    "Welcome to the Internet Relay Network <nick>!<user>@<host>"
002    RPL_YOURHOST		    "Your host is <servername>, running version <ver>"
003    RPL_CREATED		    "This server was created <date>"
004    RPL_MYINFO		    "<servername> <version> <available user modes> <available channel modes>"
005    RPL_BOUNCE		    "Try server <server name>, port <port number>"
200    RPL_TRACELINK		"Link <version & debug level> <destination> <next server> V<protocol version>		<link uptime in seconds> <backstream sendq>		<upstream sendq>"
201    RPL_TRACECONNECTING	"Try. <class> <server>"
202    RPL_TRACEHANDSHAKE	"H.S. <class> <server>"
203    RPL_TRACEUNKNOWN		"???? <class> [<client IP address in dot form>]"
204    RPL_TRACEOPERATOR	"Oper <class> <nick>"
205    RPL_TRACEUSER		"User <class> <nick>"
206    RPL_TRACESERVER		"Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server> V<protocol version>"
207    RPL_TRACESERVICE		"Service <class> <name> <type> <active type>"
208    RPL_TRACENEWTYPE		"<newtype> 0 <client name>"
209    RPL_TRACECLASS		"Class <class> <count>"
210    RPL_TRACERECONNECT	Unused.
211    RPL_STATSLINKINFO	"<linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>"
212    RPL_STATSCOMMANDS	"<command> <count> <byte count> <remote count>"
219    RPL_ENDOFSTATS		"<stats letter> :End of STATS report"
221    RPL_UMODEIS		    "<user mode string>"
242    RPL_STATSUPTIME		":Server Up %d days %d:%02d:%02d"
243    RPL_STATSOLINE		"O <hostmask> * <name>"
234    RPL_SERVLIST		    "<name> <server> <mask> <type> <hopcount> <info>"
235    RPL_SERVLISTEND		"<mask> <type> :End of service listing"
250
251    RPL_LUSERCLIENT		":There are <integer> users and <integer> services on <integer> servers"
252    RPL_LUSEROP		    "<integer> :operator(s) online"
253    RPL_LUSERUNKNOWN		"<integer> :unknown connection(s)"
254    RPL_LUSERCHANNELS	"<integer> :channels formed"
255    RPL_LUSERME		    ":I have <integer> clients and <integer> servers"
256    RPL_ADMINME		    "<server> :Administrative info"
257    RPL_ADMINLOC1		":<admin info>"
258    RPL_ADMINLOC2		":<admin info>"
259    RPL_ADMINEMAIL		":<admin info>"
261    RPL_TRACELOG		    "File <logfile> <debug level>"
262    RPL_TRACEEND		    "<server name> <version & debug level> :End of TRACE"
263    RPL_TRYAGAIN		    "<command> :Please wait a while and try again."
265
266
301    RPL_AWAY		        "<nick> :<away message>"
302    RPL_USERHOST		    ":*1<reply> *( " " <reply> )"
303    RPL_ISON		        ":*1<nick> *( " " <nick> )"
305    RPL_UNAWAY		    ":You are no longer marked as being away"
306    RPL_NOWAWAY		    ":You have been marked as being away"
311    RPL_WHOISUSER		"<nick> <user> <host> * :<real name>"
312    RPL_WHOISSERVER		"<nick> <server> :<server info>"
313    RPL_WHOISOPERATOR	"<nick> :is an IRC operator"
314    RPL_WHOWASUSER		"<nick> <user> <host> * :<real name>"
315    RPL_ENDOFWHO		    "<name> :End of WHO list"
317    RPL_WHOISIDLE		"<nick> <integer> :seconds idle"
318    RPL_ENDOFWHOIS		"<nick> :End of WHOIS list"
319    RPL_WHOISCHANNELS	"<nick> :*( ( "@" / "+" ) <channel> " " )"
321    RPL_LISTSTART		Obsolete. Not used.
322    RPL_LIST		        "<channel> <# visible> :<topic>"
323    RPL_LISTEND		    ":End of LIST"
324    RPL_CHANNELMODEIS	"<channel> <mode> <mode params>"
325    RPL_UNIQOPIS		    "<channel> <nickname>"
331    RPL_NOTOPIC		    "<channel> :No topic is set"
332    RPL_TOPIC		    "<channel> :<topic>"
341    RPL_INVITING		    "<channel> <nick>"
342    RPL_SUMMONING		"<user> :Summoning user to IRC"
346    RPL_INVITELIST		"<channel> <invitemask>"
347    RPL_ENDOFINVITELIST	"<channel> :End of channel invite list"
348    RPL_EXCEPTLIST		"<channel> <exceptionmask>"
349    RPL_ENDOFEXCEPTLIST	"<channel> :End of channel exception list"
351    RPL_VERSION		    "<version>.<debuglevel> <server> :<comments>"
352    RPL_WHOREPLY		    "<channel> <user> <host> <server> <nick> ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] :<hopcount> <real name>"
353    RPL_NAMREPLY		    "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> ) - "@" is used for secret channels, "*" for private channels, and "=" for others (public channels)."
364    RPL_LINKS		    "<mask> <server> :<hopcount> <server info>"
365    RPL_ENDOFLINKS		"<mask> :End of LINKS list"
366    RPL_ENDOFNAMES		"<channel> :End of NAMES list"
367    RPL_BANLIST		    "<channel> <banmask>"
368    RPL_ENDOFBANLIST		"<channel> :End of channel ban list"
369    RPL_ENDOFWHOWAS		"<nick> :End of WHOWAS"
371    RPL_INFO		        ":<string>"
372    RPL_MOTD		        ":- <text>"
374    RPL_ENDOFINFO		":End of INFO list"
375    RPL_MOTDSTART		":- <server> Message of the day - "
376    RPL_ENDOFMOTD		":End of MOTD command"
381    RPL_YOUREOPER		":You are now an IRC operator"
382    RPL_REHASHING		"<config file> :Rehashing"
383    RPL_YOURESERVICE		"You are service <servicename>"
391    RPL_TIME		        "<server> :<string showing server's local time>"
392    RPL_USERSSTART		":UserID   Terminal  Host"
393    RPL_USERS		    ":<username> <ttyline> <hostname>"
394    RPL_ENDOFUSERS		":End of users"
395    RPL_NOUSERS		    ":Nobody logged in"
