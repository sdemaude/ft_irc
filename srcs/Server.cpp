/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 15:24:43 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool Server::_running = true;

Server::Server(short port, std::string &password) : _port(port), _password(password) {
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
	//TODO? close all the clients if the server is stopped

	// Close the socket
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
	Client client(client_fd, client_ip);
	this->_clients.insert(std::pair<int, Client>(client_fd, client));

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

	parse_message(this->_clients.find(fd)->second, message);
}

std::string	Server::read_message(int fd) {
	char		buffer[BUFFER_SIZE];
	std::string	message = "";

	memset(&buffer, '\0', BUFFER_SIZE);
	while (1) {
		int	byte_read = recv(fd, buffer, BUFFER_SIZE, 0);

		if (byte_read < 0)
			return "";

		buffer[byte_read] = '\0';
		message += buffer;

		if (byte_read < BUFFER_SIZE)
			return message;
	}
}

static std::vector<std::string>	split_string(std::string str, char delimiter) {
	std::vector<std::string>	string_splitted;
	size_t						start = 0;
	size_t						end = str.find(delimiter);

	while (end != std::string::npos) {
		string_splitted.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}

	string_splitted.push_back(str.substr(start, str.size() - start));

	return string_splitted;
}

void	Server::parse_message(Client &client, std::string message) {
	message = client.get_buffer() + message;
	std::cout << "[" << client.getFd() << "] Message received : " << message << std::endl;

	client.clear_buffer();

	std::vector<std::string>	splitted_message = split_string(message, '\n');

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
		std::vector<std::string>	splitted_command = split_string(splitted_message[i], ' ');

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
	std::cout << "[" << client.getFd() << "] Command received : " << command << std::endl;
	std::cout << "[" << client.getFd() << "] Params received : " << params << std::endl;

	//TODO? check if the client is registered
	//TODO? check if the command is valid

	if (command == "PASS") {
		//TODO? check if the password is correct
	}

	else if (command == "NICK") {

	}

	else if (command == "USER") {

	}

	else if (command == "PRIVMSG") {

	}

	else if (command == "PING") {

	}

	else if (command == "QUIT") {

	}

	// Channel
	else if (command == "JOIN") {

	}

	else if (command == "INVITE") {

	}

	else if (command == "KICK") {

	}

	else if (command == "TOPIC") {

	}

	else if (command == "MODE") {

	}
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
		//if (nfds == -1)
			//return (perror("epoll_wait"), EXIT_FAILURE);
		// Loop through the events
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == this->_socket_fd) {
				handle_connection();
			} else {
				std::cout << "Client trying to send a message" << std::endl;
				handle_message(events[i].data.fd);
				// Read the message using recv()
				//	- If the message is not empty, parse it and send the response
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
	int i = 1; //TODO change name
	status = setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int)); // TODO? add SO_REUSEPORT if needed
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

std::string Server::getIpAdress() const {
	return (inet_ntoa(this->_addr.sin_addr));
}
