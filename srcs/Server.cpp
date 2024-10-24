/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/24 18:18:41 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(std::string port, std::string password) : _password(password){
    // Check if port is a number
    try {
        _port = std::stoi(port);
    } catch (std::exception &e) {
        std::cerr << "Error: Port must be a number" << std::endl;
        return;
    }

	// Check if port is between 1024 and 65535
	if (_port < 1024 || _port > 65535) {
		std::cerr << "Error: Port must be between 1024 and 65535" << std::endl;
		return;
	}

	//TODO? check if password is empty

	// Set the address structure
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(this->_port);

	// Start the server
	if (this->start() || this->loop())
		return;
}

Server::~Server() {
	// Close the socket
	close(this->_socket_fd);
	std::cout << "Socket closed" << std::endl;
}

int Server::loop() {
	// Handle signals
	std::signal(SIGINT, Server::handle_signal);

	// Create the epoll
	this->_epoll_fd = epoll_create(BACKLOG); //TODO? change BACKLOG to a better value
	if (this->_epoll_fd == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	
	// Add the server socket to the epoll
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = this->_socket_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_socket_fd, &ev) == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}

	// Wait for events and connect clients using the Client class
	struct epoll_event events[MAX_EVENTS];
	while (running) {
		int nfds = epoll_wait(this->_epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return EXIT_FAILURE;
		}

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == this->_socket_fd) {
				// Accept the connection and add the client to the epoll using the Client class
			} else {
				// Read the message
				//	- If the message is empty, close the connection and remove the client from the epoll
				//	- If the message is not empty, parse it and send the response
			}
		}
	}

	//TODO? close all the clients if the server is stopped

	return EXIT_SUCCESS;
}

int Server::start() {
	// Create the socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Socket created" << std::endl;

	// Allow to reuse the port
	int status = setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); // TODO? add SO_REUSEPORT if needed
	if (status == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}

	// Bind the socket
	status = bind(this->_socket_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr));
	if (status == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Socket binded on port : " << this->_port << std::endl;

	// Listen on the socket
	status = listen(this->_socket_fd, BACKLOG);
	if (status == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Socket listening on port : " << this->_port << std::endl;

	return EXIT_SUCCESS;
}

void Server::handle_signal(int signal) {
	if (signal == SIGINT) {
		running = false;
		std::cout << "Server stopped, " << signal << " received" << std::endl;
	}
}