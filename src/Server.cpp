#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstring>
#include <algorithm>

#include "Server.hpp"
#include "User.hpp"

namespace ft {

Server::Server(int port)
	: _users(), _pollfds(), _socket(-1)
{
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0) {
		throw std::string("Could not create socket: ") + ::strerror(errno);
	}

	int opt = 1;
	if (::setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw std::string("Could not set socket option 'SO_REUSEADDR': ") + ::strerror(errno);
	}

	if(::fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
		throw std::string("Could not enable non-blocking mode on socket: ") + ::strerror(errno);
	}

	struct sockaddr_in address;

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if(::bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw std::string("Could not bind address to socket: ") + ::strerror(errno);
	}

	if(::listen(this->_socket, 99) < 0) {
		throw std::string("Could not listen on socket: ") + ::strerror(errno);
	}

	std::cout << "IRC Server listening on port " << port << std::endl;
	this->setup_commands();
}

void Server::setup_commands()
{
	this->_commands["HELP"] = new cmd::Help(*this);
	this->_commands["JOIN"] = new cmd::Join(*this);
	this->_commands["PART"] = new cmd::Part(*this);
	this->_commands["PING"] = new cmd::Ping(*this);
	this->_commands["PRIVMSG"] = new cmd::Privmsg(*this);
	this->_commands["QUIT"] = new cmd::Quit(*this);
	this->_commands["WHO"] = new cmd::Who(*this);
}

Server::~Server()
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		delete this->_users[i];
	}
	for (std::map<std::string, Command *>::iterator it = this->_commands.begin(); it != this->_commands.end(); ++it) {
		delete it->second;
	}
	::close(this->_socket);
}

void Server::poll()
{

	for (size_t i = 0; i < this->_pollfds.size(); ++i) {
		struct pollfd&	pfd = this->_pollfds[i];
		User			*user = this->_users[i];

		if (user->response_queue_size() > 0) {
			pfd.events |= POLLOUT;
		} else {
			pfd.events &= ~POLLOUT;
		}
	}

	int poll_ret = ::poll(&this->_pollfds[0], this->_pollfds.size(), 30);
	if (poll_ret < 0) {
		throw std::string("Could not poll on socket: ") + ::strerror(errno);
	}

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = ::accept(this->_socket, (struct sockaddr*)&addr, &addrlen);
	if (client_fd >= 0) {
		User *user = new User(client_fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), &*this);
		std::cout << user->host() << ":" << user->port() << " logged in" << std::endl;
		this->_users.push_back(user);
		this->_pollfds.push_back((struct pollfd) {
			.fd = client_fd,
			.events = POLLIN | POLLHUP,
			.revents = 0
		});
	}

	char buf[1024];
	for (size_t i = 0; i < this->_pollfds.size(); ++i) {
		const struct pollfd&	pfd = this->_pollfds[i];
		User					*user = this->_users[i];
		if (pfd.revents & POLLIN) {
			std::memset(buf, 0, 1023);
			int n = recv(pfd.fd, buf, 1023, MSG_PEEK);
			if (n > 0) {
				if (char *pos = std::strstr(buf, "\n")) {
					size_t len = pos - (char*)buf;
					std::memset(buf, 0, 1023);
					n = recv(pfd.fd, buf, len + 1, 0);
				} else {
					n = recv(pfd.fd, buf, 1023, 0);
				}
				user->append_to_message(buf);
			} else {
				this->disconnect(i);
				if (i-- == 0) {
					break ;
				}
				continue ;
			}
		}
		if (pfd.revents & POLLOUT) {
			if (user->response_queue_size() == 0) {
				/* TODO: Should not happen, but if it happens it is VERY concerning */
			}
			std::string message = user->response_queue_pop();
			if (::send(user->socket(), &message[0], message.size(), 0) < 0) {
				this->disconnect(i);
				if (i-- == 0) {
					break ;
				}
				continue ;
			}
		}
		if (pfd.revents & POLLHUP) {
			this->disconnect(i);
			if (i-- == 0) {
				break ;
			}
		}
	}
}

void Server::disconnect(size_t user_index)
{
	const User *user = this->_users[user_index];
	std::cout << user->host() << ":" << user->port() << " has lost connection" << std::endl;
	this->_users[user_index]->disconnect();
	delete this->_users[user_index];
	this->_users.erase(this->_users.begin() + user_index);
	this->_pollfds.erase(this->_pollfds.begin() + user_index);
}

void Server::process_message(User& sender, const std::string& message)
{
	std::string command;
	std::vector<std::string> params;

	try {
		this->parse_message(message, command, params);
	} catch (const std::string& err) {
		std::cerr << "WARNING: Invalid message: " << err << std::endl;
		return ;
	}

	if (command.empty()) {
		std::cerr << "WARNING: Empty message";
		return ;
	}

	/* Put command in uppercase, just in case */
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	try {
		Command *cmd = this->_commands.at(command);
		cmd->execute(&sender, params);
	} catch (std::exception& e) {
		(void)e;
		this->send_error(sender, std::string() + ERR_UNKNOWNCOMMAND(command));
	} catch (const std::string& err) {
		this->send_error(sender, err);
	}
}

void Server::send_error(User& user, const std::string& err)
{
	std::cerr << "Unhandled error to " << user.host() << ":" << user.port() << ": " << err << std::endl;
}

int	 Server::find_user_index(const User& user)
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		if (this->_users[i]->socket() == user.socket()) {
			return i;
		}
	}
	return -1;
}

void Server::parse_message(const std::string& message, std::string& command, std::vector<std::string>& params)
{
	const char *str = &message[0]; /* TODO: unused */
	command.clear();
	params.clear();

	std::vector<std::string> space_segments;
	std::vector<std::string> colon_segments;

	std::stringstream stream0(message);
	std::stringstream stream1(message);
	std::string segment;

	bool first = true;
	while(std::getline(stream0, segment, ' '))
	{
		if (first) {
			command = segment;
			first = false;
			continue ;
		}
		space_segments.push_back(segment);
	}

	if (space_segments.empty() && command.empty()) {
		throw std::string("empty command");
	}

	if (std::strstr(&command[0], ":")) {
		throw std::string("cannot have colons in command name");
	}

	while(std::getline(stream1, segment, ':'))
	{
		colon_segments.push_back(segment);
	}

	for (std::vector<std::string>::const_iterator it = space_segments.begin(); it != space_segments.end(); ++it) {
		if (std::strstr(&(*it)[0], ":")) {
			if ((*it)[0] != ':') {
				throw std::string("colon in middle of argument");
			}
			break ;
		}
		params.push_back(*it);
	}

	for (std::vector<std::string>::const_iterator it = colon_segments.begin(); it != colon_segments.end(); ++it) {
		if (it == colon_segments.begin()) {
			continue ;
		}
		if (it->empty()) {
			throw std::string("two colons not separated by space");
		}
		if ((*it)[it->size() - 1] != ' ') {
			if (it != (colon_segments.end() - 1)) {
				throw std::string("no space before colon");
			}
		}
		params.push_back(*it);
	}

	for (size_t i = 0; i < params.size(); ++i) {
		std::cout << params[i] << std::endl;
	}
}

void	Server::create_channel(User *creator, const std::vector<std::string> &args) /* TODO: make it cleaner */
{
	if (args.size() == 1) {
		this->_channels[args[0]] = new Channel(creator, args[0]);
	} else if (args.size() == 2) {
		this->_channels[args[0]] = new Channel(creator, args[0], args[1]);
	} else {
		throw; /* TODO: error invalid number of args */
	}
}

}
