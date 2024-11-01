/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/30 09:07:11 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool Server::_running = true;

Server::Server(int port, std::string password) : _port(port), _password(password) {
	this->_socket_fd = 0;
	this->_epoll_fd = 0;

	// Set the address structure
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(this->_port);

	// Start the server
	if (this->init() || this->loop())
		throw std::runtime_error("Server failed to start");
}

Server::~Server() {
	// Delete all the clients
	while (!this->_clients.empty()) {
		delete this->_clients.begin()->second;
		this->_clients.erase(this->_clients.begin());
	}
	// Delete all the channels
	while (!this->_channels.empty()) {
		delete this->_channels.begin()->second;
		this->_channels.erase(this->_channels.begin());
	}
	// Close the server socket
	close(this->_socket_fd);
	std::cout << "Server socket closed" << std::endl;
	std::cout << "Server stopped after receiving SIGINT" << std::endl;
}

void Server::handle_connection() {
	std::cout << "New client trying to connect" << std::endl;

	// Accept the connection
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd = accept(this->_socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd == -1) {
		perror("accept");
		return;
	}

	// Convert the client's IP address to a string
	char client_ip[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
		perror("inet_ntop");
		close(client_fd);
		return;
	}

	// Add the client to the map
	Client	*client = new Client(client_fd, client_ip);
	this->_clients.insert(std::pair<int, Client *>(client_fd, client));

	// Set the client to non-blocking
	int status = fcntl(client_fd, F_SETFL, O_NONBLOCK);
	if (status == -1) {
		perror("fcntl");
		close(client_fd);
		return;
	}

	// Add the client to the epoll
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = client_fd;
	status = epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
	if (status == -1) {
		perror("epoll_ctl");
		close(client_fd);
		return;
	}

	std::cout << "Unregistered client connected with IP : " << client_ip << std::endl;
}

void	Server::handle_message(int fd) {
	std::string	message = read_message(fd);
	if (message == "") return;

	parse_message(*(this->_clients.find(fd)->second), message);
}

std::string	Server::read_message(int fd) {
	char		buffer[BUFFER_SIZE + 1];
	std::string	message = "";

	memset(&buffer, '\0', BUFFER_SIZE);
	while (1) {
		int	byte_read = recv(fd, buffer, BUFFER_SIZE, 0);

		if (byte_read < 0) {
			if (errno != EAGAIN)
				perror("recv");
			return (message);
		}

		buffer[byte_read] = '\0';
		message += buffer;

		if (byte_read < BUFFER_SIZE)
			return message;
	}
}

static std::vector<std::string> split_string(std::string str, char delimiter, bool erase_delimiter) {
		// Split a string into a vector of strings using a delimiter
		std::vector<std::string> string_splitted;
		size_t start = 0;
		size_t end = str.find(delimiter);
	
		while (end != std::string::npos) {
			if (erase_delimiter) {
				string_splitted.push_back(str.substr(start, end - start)); // Exclude the delimiter
			} else {
				string_splitted.push_back(str.substr(start, end - start + 1)); // Include the delimiter
			}
			start = end + 1;
			end = str.find(delimiter, start);
		}
	
		string_splitted.push_back(str.substr(start, str.size() - start)); // Add the remaining part
	
		return string_splitted;
}

void	Server::parse_message(Client &client, std::string message) {
	message = client.get_buffer() + message;
	std::cout << std::endl << "Message received : " << message;

	client.clear_buffer();

	std::vector<std::string>	splitted_message = split_string(message, '\n', false);

	for (size_t i = 0; i < splitted_message.size(); i++) {
		if (splitted_message[i].find("\r\n") == std::string::npos) {
			client.set_buffer(splitted_message[i]);
			return ;
		}

		splitted_message[i].resize(splitted_message[i].size() - 2);
		if (splitted_message[i].size() == 0)
			continue;
		
		std::string					prefix = "";
		std::string					command = "";
		std::string					params = "";
		std::vector<std::string>	splitted_command = split_string(splitted_message[i], ' ', true);

		if (splitted_command.size() == 0)
			continue;

		size_t	j = 0;

		if (splitted_command[j][0] == ':')
			prefix = splitted_command[j++];

		if (splitted_command.size() > j)
			command = splitted_command[j++];

		if (splitted_command.size() > j) {
			params = splitted_command[j++];
			while (splitted_command.size() > j)
				params += " " + splitted_command[j++];
		}

		this->parse_command(client, prefix, command, params);
	}
}

void	Server::parse_command(Client &client, std::string prefix, std::string command, std::string params) {
	(void)prefix;
	std::cout << "[" << client.getFd() << "] Command received : " << command << std::endl;
	std::cout << "[" << client.getFd() << "] Params received : " << params << std::endl;

	std::vector<std::string>	splitted_params = split_string(params, ' ', true);

	if (command == "PASS") {
		this->pass(client, params);
	} else if (command == "NICK") {
		this->nick(client, params);
	} else if (command == "USER") {
		if (splitted_params.size() < 4) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string	username = splitted_params[0];
		std::string	hostname = splitted_params[1];
		std::string	realname = splitted_params[3];

		if (realname[0] == ':') {
			realname = realname.substr(1);

			if (splitted_params.size() > 4) {
				for (size_t i = 4; i < splitted_params.size(); i++)
					realname += " " + splitted_params[i];
			}
		}

		this->user(client, username, hostname, realname);
	} else if (command == "PRIVMSG" && client.getRegistered()) {
		if (splitted_params.size() < 2) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string	target = splitted_params[0];
		std::string	message = splitted_params[1];

		if (message[0] == ':') {
			message = message.substr(1);

			if (splitted_params.size() > 2) {
				for (size_t i = 2; i < splitted_params.size(); i++)
					message += " " + splitted_params[i];
			}
		}

		this->privmsg(client, target, message);
	} else if (command == "PING" && client.getRegistered()) {
		this->ping(client);
	} else if (command == "QUIT" && client.getRegistered()) {
		this->quit(client);
	}

	// Channel

	else if (command == "JOIN" && client.getRegistration()) {
		// Check if the client is registered
		if (params.size() == 0) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		// Get the channel name
		std::string									channel_name = splitted_params[0];
		std::map<std::string, Channel *>::iterator	it = this->_channels.find(channel_name);

		// If the channel does not exist, create it and add the client to it
		if (it == this->_channels.end()) {
			Channel	*channel = new Channel(channel_name);
			this->_channels.insert(std::pair<std::string, Channel *>(channel_name, channel));

			//TODO : add client to channel here if the channel does not exist
			channel->add_client(client);
			client.addChannel(channel_name);
			//client.getChannels().insert(std::pair<std::string, Channel>(channel.getName(), channel));
			std::string response = ":" + client.getId() + " JOIN " + channel->getName() + "\r\n";
			channel->sendToAll(response);
			std::cout << "Client created and joined the channel" << std::endl;
			return;
		}

		std::string	password = (splitted_params.size() > 1) ? splitted_params[1] : "";

		// Add the client to the channel
		this->join(client, *(*it).second, password);
	} else if (command == "PART" && client.getRegistered()) {
		if (params.size() == 0) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string									channel_name = splitted_params[0];
		std::map<std::string, Channel *>::iterator	it = this->_channels.find(channel_name);

		if (it == this->_channels.end()) {
			std::string	response = ":server " + ERR_NOSUCHCHANNEL + " " + client.getNickname() + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		this->part(client, *(*it).second);
	} else if (command == "INVITE" && client.getRegistered()) {
		if (splitted_params.size() < 2) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string						target_nick = splitted_params[0];
		std::string						channel_name = splitted_params[1];
		std::map<int, Client *>::iterator	it_target = this->_clients.find(getFdByNickname(target_nick));

		if (it_target == this->_clients.end()) {
			std::string	response = ":server " + ERR_NOSUCHNICK + " " + client.getNickname() + " :No such nick/channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::map<std::string, Channel *>::iterator	it_channel = this->_channels.find(channel_name);

		if (it_channel == this->_channels.end()) {
			std::string	response = ":server " + ERR_NOSUCHCHANNEL + " " + client.getNickname() + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		this->invite(client, *(*it_channel).second, *(*it_target).second);
	} else if (command == "KICK" && client.getRegistered()) {
		if (splitted_params.size() < 2) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string							target_nick = splitted_params[1];
		std::string							channel_name = splitted_params[0];
		std::map<int, Client *>::iterator	it_target = this->_clients.find(getFdByNickname(target_nick));

		if (it_target == this->_clients.end()) {
			std::string	response = ":server " + ERR_NOSUCHNICK + " " + client.getNickname() + " :No such nick/channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::map<std::string, Channel *>::iterator	it_channel = this->_channels.find(channel_name);

		if (it_channel == this->_channels.end()) {
			std::string	response = ":server " + ERR_NOSUCHCHANNEL + " " + client.getNickname() + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		this->kick(client, *(*it_channel).second, *(*it_target).second);
	} else if (command == "TOPIC" && client.getRegistered()) {
		if (params.size() == 0) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string									channel_name = splitted_params[0];
		std::string									topic = "";
		std::map<std::string, Channel *>::iterator	it = this->_channels.find(channel_name);

		if (it == this->_channels.end()) {
			std::string	response = ":server " + ERR_NOSUCHCHANNEL + " " + client.getNickname() + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		if (splitted_params.size() > 1) {
			topic = splitted_params[1];

			if (topic[0] == ':') {
				topic = topic.substr(1);

				if (splitted_params.size() > 2) {
					for (size_t i = 2; i < splitted_params.size(); i++)
						topic += " " + splitted_params[i];
				}
			}
		}

		bool	view_topic = (splitted_params.size() == 1);

		this->topic(client, *(*it).second, topic, view_topic);
	} else if (command == "MODE" && client.getRegistered()) {
		// client, channel, mode, mode parameter or empty
		if (splitted_params.size() < 2) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string									channel_name = splitted_params[0];
		std::map<std::string, Channel *>::iterator	it = this->_channels.find(channel_name);
		std::string									mode = splitted_params[1];

		if (it == this->_channels.end()) {
			std::map<int, Client *>::iterator	it_client = this->_clients.find(getFdByNickname(channel_name));
			if (it_client == this->_clients.end()) {
				std::string	response = ":server " + ERR_NOSUCHCHANNEL + " " + client.getNickname() + " :No such channel\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
				return;
			}
			return;
		}

		if (mode.size() != 2
			|| (mode[0] != '+' && mode[0] != '-')
			|| (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'k' && mode[1] != 'o' && mode[1] != 'l')) {
			std::string	response = ":server " + ERR_UNKNOWNMODE + " " + client.getNickname() + " :is unknown mode char to me for " + channel_name + "\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		if (mode[0] == '+' && mode[1] != 'i' && mode[1] != 't' && splitted_params.size() < 3) {
			std::string	response = ":server " + ERR_NEEDMOREPARAMS + " " + client.getNickname() + " :Not enough parameters\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			return;
		}

		std::string	parameter = (mode[0] == '+' && mode[1] != 'i' && mode[1] != 't') ? splitted_params[2] : "";
		if (mode == "+t")
			parameter = "+t";
		if (mode == "-o")
			parameter = splitted_params[2];
		
		bool	set;
		if (mode[0] == '+')
			set = true;
		else if (mode[0] == '-')
			set = false;

		this->mode(client, *(*it).second, mode[1], parameter, set);
	}
}

int	Server::getFdByNickname(std::string &nickname) {
	std::map<int, Client *>::iterator	it = this->_clients.begin();

	while (it != this->_clients.end()) {
		if (it->second->getNickname() == nickname)
			return it->first;
		it++;
	}
	return -1;
}

int Server::loop() {
	// Handle CTRL+C signal
	std::signal(SIGINT, Server::handle_signal);

	// Create the epoll
	this->_epoll_fd = epoll_create1(0);
	if (this->_epoll_fd == -1)
		return (perror("epoll_create1"), EXIT_FAILURE);

	// Add the server socket to the epoll
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = this->_socket_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_socket_fd, &ev) == -1)
		return (perror("epoll_ctl"), EXIT_FAILURE);

	// Wait for events and connect clients using the Client class
	struct epoll_event events[MAX_EVENTS];
	while (Server::_running) {
		int nfds = epoll_wait(this->_epoll_fd, events, MAX_EVENTS, -1);
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == this->_socket_fd) {
				handle_connection();
			} else {
				handle_message(events[i].data.fd);
			}
		}
	}
	return EXIT_SUCCESS;
}

int Server::init() {
	// Create the socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1)
		return (perror("socket"), EXIT_FAILURE);
	std::cout << "Socket created" << std::endl;

	// Set the socket options using fcntl
	int status = fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK);
	if (status == -1)
		return (perror("fcntl"), EXIT_FAILURE);

	// Allow to reuse the port
	int reuse_addr = 1;
	status = setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int));
	if (status == -1)
		return (perror("setsockopt"), EXIT_FAILURE);

	// Bind the socket
	status = bind(this->_socket_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr));
	if (status == -1)
		return (perror("bind"), EXIT_FAILURE);
	std::cout << "Socket binded on port : " << this->_port << std::endl;

	// Listen on the socket
	status = listen(this->_socket_fd, BACKLOG);
	if (status == -1)
		return (perror("listen"), EXIT_FAILURE);
	std::cout << "Socket listening on port : " << this->_port << std::endl;

	return EXIT_SUCCESS;
}

void Server::handle_signal(int signal) {
	if (signal == SIGINT) {
		Server::_running = false;
	}
}
