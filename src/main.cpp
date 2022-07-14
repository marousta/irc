#include "Server.hpp"

int main()
{
	ft::Server *server;

	try {
		server = new ft::Server(6667);
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
}
