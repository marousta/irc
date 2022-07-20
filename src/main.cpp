#include <cstdlib>
#include <cstring>

#include "Server.hpp"

bool	check_digit(const char *str)
{
	for (size_t i = 0; i < strlen(str); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

bool	parse_args(int ac, char **av, int *port, std::string *pass)
{
	if (ac == 1) {
		return true;
	}
	if (ac > 3) {
		return false;
	}

	/* count successfully parsed args */
	bool args[2] = {false};

	if (ac >= 2) {
		for (int i = 1; i < ac; i++) { /* unordered args, iterating over them */
			if (check_digit(av[i])) {
				if (args[0]) { /* arg already set: error */
					return false;
				}
				/* convert into int, check and set */
				int tmp = atoi(av[i]);
				if (tmp < 0 || tmp > 65535) {
					return false;
				}
				*port = tmp;
				args[0] = true;
			} else {
				if (args[1]) { /* arg already set: error */
					return false;
				}
				*pass = std::string(av[i]);
				args[1] = true;
			}
		}
	}
	return true;
}

int	main(int ac, char **av)
{
	int port = 6667;
	std::string pass = "";

	if (!parse_args(ac, av, &port, &pass)) {
		throw std::string("Error in arguments\n");
	}

	ft::Server *server;

	try {
		server = new ft::Server(port, pass);
	} catch (const std::string& err) {
		std::cerr << err << std::endl;
		return -1;
	}

	try {
		for (;;) {
			server->poll();
		}
	} catch (const std::string& err) {
		std::cerr << err << std::endl;
		return -2;
	}

	delete server;
	server = NULL;
}
