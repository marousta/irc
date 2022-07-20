#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstring>
#include <algorithm>

#include "Colors.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace ft {

Server::Server(int port, std::string pass)
	: _pass(pass), _users(), _pollfds(), _socket(-1)
{
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0) {
		throw std::string(RED "Could not create socket: ") + ::strerror(errno) + COLOR_RESET;
	}

	int opt = 1;
	if (::setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw std::string(RED "Could not set socket option 'SO_REUSEADDR': ") + ::strerror(errno) + COLOR_RESET;
	}

	if(::fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
		throw std::string(RED "Could not enable non-blocking mode on socket: ") + ::strerror(errno) + COLOR_RESET;
	}

	struct sockaddr_in address;

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if(::bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw std::string(RED "Could not bind address to socket: ") + ::strerror(errno) + COLOR_RESET;
	}

	if(::listen(this->_socket, 99) < 0) {
		throw std::string(RED "Could not listen on socket: ") + ::strerror(errno) + COLOR_RESET;
	}

	std::cout << "IRC Server listening on port " << port << std::endl;
	if (pass != "") {
		std::cout << "Password: " << pass << std::endl;
	}
	this->setup_commands();
	this->setup_ignored_commands();
}

Server::~Server()
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		delete this->_users[i];
	}
	for (std::map<std::string, Command *>::iterator it = this->_commands.begin(); it != this->_commands.end(); ++it) {
		delete it->second;
	}
	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		delete it->second;
	}
	::close(this->_socket);
}

bool	Server::check_pass(std::string pass) const
{
	if (!this->_pass.size()) {
		return true;
	}
	return this->_pass == pass;
}

void	Server::poll()
{
	for (	std::vector<User *>::const_iterator it = this->_disconnect_requests.begin();
			it != this->_disconnect_requests.end();
			++it)
	{
		int index = this->find_user_index(**it);
		if (index >= 0) {
			this->disconnect(index);
		}
	}

	this->_disconnect_requests.clear();

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
		throw std::string(RED "Could not poll on socket: ") + ::strerror(errno) + COLOR_RESET;
	}

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = ::accept(this->_socket, (struct sockaddr*)&addr, &addrlen);
	if (client_fd >= 0) {
		User *user = new User(client_fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), this, this->_pass == "");
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
				this->request_disconnect(i);
				if (i-- == 0) {
					break ;
				}
				continue ;
			}
		}
		if (pfd.revents & POLLOUT) {
			if (user->response_queue_size() == 0) {
				std::cerr << RED "WARNING: user response queue is empty. THIS SHOULD NOT HAPPEN !" COLOR_RESET << std::endl;
			}
			std::string message = user->response_queue_pop();
			if (::send(user->socket(), &message[0], message.size(), 0) < 0) {
				this->request_disconnect(i);
				if (i-- == 0) {
					break ;
				}
				continue ;
			}
			std::cout << BLU "SERVER " COLOR_RESET << message;
		}
		if (pfd.revents & POLLHUP) {
			this->request_disconnect(i);
			if (i-- == 0) {
				break ;
			}
		}
	}
}

void	Server::request_disconnect(size_t user_index)
{
	User *user = this->_users[user_index];
	this->_disconnect_requests.push_back(user);
}

void	Server::disconnect(size_t user_index)
{
	User *user = this->_users[user_index];

	std::vector<Channel *> channels;

	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		channels.push_back(it->second);
	}

	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
		(*it)->remove_user(user);
	}

	std::cout << user->host() << ":" << user->port() << YEL " has lost connection" COLOR_RESET << std::endl;
	this->_users[user_index]->disconnect();
	delete this->_users[user_index];
	this->_users.erase(this->_users.begin() + user_index);
	this->_pollfds.erase(this->_pollfds.begin() + user_index);
}

void	Server::process_message(User& sender, const std::string& message)
{
	std::cout << GRN "CLIENT " COLOR_RESET << message << std::endl;

	std::string command;
	std::vector<std::string> params;

	const char *msg = &message[0];
	while (*msg && *msg != ' ') {
		command += *msg++;
	}
	while (*msg == ' ') {
		++msg;
	}

	/* Put command in uppercase, just in case */
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if (command.empty()) {
		std::cerr << RED "WARNING: Empty message" COLOR_RESET << std::endl;
		return ;
	}
	if (::strstr(&command[0], ":")) {
		std::cerr << RED "WARNING: Invalid message (colon in command)" COLOR_RESET << std::endl;
		return ;
	}

	Command *cmd = 0;
	try {
		cmd = this->_commands.at(command);
		cmd->execute(&sender, msg);
	} catch (std::exception&) {
		for (std::vector<std::string>::iterator it = this->_ignored_commands.begin(); it != this->_ignored_commands.end(); ++it) {
			if (command == *it) {
				return ;
			}
		}
		this->send_error(sender, ERR_UNKNOWNCOMMAND(command));
		return ;
	} catch (const std::string& err) {
		this->send_error(sender, err);
		return ;
	}
}

int		Server::find_user_index(const User& user)
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		if (this->_users[i]->socket() == user.socket()) {
			return i;
		}
	}
	return -1;
}

User*	Server::get_user_username(const std::string& username) const
{
	int index = this->find_user_username(username);
	if (index == -1) {
		return NULL;
	}
	return this->_users[index];
}

User*	Server::get_user_nick(const std::string& nick) const
{
	int index = this->find_user_nick(nick);
	if (index == -1) {
		return NULL;
	}
	return this->_users[index];
}

size_t	Server::user_count(void) const
{
	size_t count = 0;

	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->registered()) {
			++count;
		}
	}
	return count;
}

void	Server::create_channel(User *creator, const std::string& channel_name, const std::string& key)
{
	if (key.empty()) {
		std::cout << YEL "Creating channel " COLOR_RESET << channel_name << YEL "... " COLOR_RESET;
		this->_channels[channel_name] = new Channel(creator, channel_name, this);
	} else {
		std::cout << YEL "Creating channel " COLOR_RESET << channel_name << YEL " with key... " COLOR_RESET;
		this->_channels[channel_name] = new Channel(creator, channel_name, key, this);
	}
}

void	Server::remove_channel(const std::string& name)
{
	try {
		std::map<std::string, Channel *>::iterator channel = this->_channels.find(name);
		delete channel->second;
		this->_channels.erase(channel);
	}
	catch(const std::exception&) {	}
}

void	Server::join_channel(User *user, const std::string& channel_name, const std::string& key)
{
	try {
		Channel *channel = this->_channels.at(channel_name);

		if (channel->mode() & MODE_K) {
			if (!key.empty() && channel->key_compare(key)) {
				channel->add_user(user);
			} else {
				throw ERR_BADCHANNELKEY(channel_name);
			}
		} else {
			channel->add_user(user);
		}
	}
	catch (std::exception &e) {
		(void)e;
		this->create_channel(user, channel_name, key);
	}
}

size_t	Server::channel_count(void) const
{
	return this->_channels.size();
}


Channel*	Server::get_channel_with_name(const std::string& name)
{
	try {
		return this->_channels.at(name);
	}
	catch (std::exception &e) {
		(void)e;
		throw ERR_NOSUCHCHANNEL(name);
	}
}

std::vector<Channel *>	Server ::get_channels() const
{
	std::vector<Channel *> ret;

	for (std::map<std::string, Channel *>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		ret.push_back(it->second);
	}
	return ret;
}

std::vector<Channel *> 	Server::get_channels_with_user(User *user)
{
	std::vector<Channel *> ret;

	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		if (it->second->user_exist(user)) {
			ret.push_back(it->second);
		}
	}
	return ret;
}

void	Server::print_debug(User *sender) const
{
	if (this->_pass.size()) {
		sender->send("PASSWORD: True");
	} else {
		sender->send("PASSWORD: False");
	}

	sender->send("USERS: " + convert_string(this->_users.size()));

	std::string users_list = "{\n";
	for (std::vector<User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->registered()) {
			users_list += "\t" + (*it)->nick() + "[" + (*it)->username() + "],\n";
		} else {
			users_list += "\tunknown,\n";
		}
	}
	users_list += "}";
	if (this->_users.size()) {
		sender->send(users_list);
	}

	sender->send("CHANNELS: " + convert_string(this->_channels.size()));

	std::string channel_list = "{\n";
	for (std::map<std::string, Channel *>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		std::string mode = "";
		if (it->second->mode() & MODE_K) {
			mode += "k";
		}
		if (it->second->mode() & MODE_T) {
			mode += "t[" + it->second->topic() + "]";
		}
		channel_list += "\t" + it->first + " (" + (!mode.size() ? "Default" : mode) + "),\n";
	}
	channel_list += "}";
	if (this->_channels.size()) {
		sender->send(channel_list);
	}
}

std::map<std::string, Command *>	Server::get_commands() const
{
	return this->_commands;
}

int		Server::find_user_username(const std::string& username) const
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		if (this->_users[i]->username() == username) {
			return i;
		}
	}
	return -1;
}

int		Server::find_user_nick(const std::string& nick) const
{
	for (size_t i = 0; i < this->_users.size(); ++i) {
		if (this->_users[i]->nick() == nick) {
			return i;
		}
	}
	return -1;
}

void	Server::setup_commands()
{
	this->_commands["DEBUG"] = new cmd::Debug(*this);

	this->_commands["HELP"] = new cmd::Help(*this);
	this->_commands["JOIN"] = new cmd::Join(*this);
	this->_commands["LIST"] = new cmd::List(*this);
	this->_commands["MODE"] = new cmd::Mode(*this);
	this->_commands["NICK"] = new cmd::Nick(*this);
	this->_commands["PART"] = new cmd::Part(*this);
	this->_commands["PASS"] = new cmd::Pass(*this);
	this->_commands["PING"] = new cmd::Ping(*this);
	this->_commands["PRIVMSG"] = new cmd::Privmsg(*this);
	this->_commands["QUIT"] = new cmd::Quit(*this);
	this->_commands["TOPIC"] = new cmd::Topic(*this);
	this->_commands["USER"] = new cmd::User(*this);
}

void	Server::setup_ignored_commands()
{
	this->_ignored_commands.push_back("CAP");
	this->_ignored_commands.push_back("WHO");
	this->_ignored_commands.push_back("USERHOST");
	this->_ignored_commands.push_back("PONG");
}

void	Server::send_error(User& user, const std::string& err)
{
	user.send(err);
}

}
