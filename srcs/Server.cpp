/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 11:28:56 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool Server::_running = true;

Server::Server(short port, std::string password) : _port(port), _password(password){
   	// Set the address structure
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(this->_port);

	// Start the server
	if (this->init() || this->loop())
		return;
}

Server::~Server() {
	//TODO? close all the clients if the server is stopped

	// Close the socket
	close(this->_socket_fd);
	std::cout << "Server socket closed" << std::endl;
	std::cout << "Server stopped after receiving SIGINT" << std::endl;
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
		if (nfds == -1)
			return (perror("epoll_wait"), EXIT_FAILURE);
		// Loop through the events
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == this->_socket_fd) {
				std::cout << "New client trying to connect" << std::endl;
				// Accept the connection and add the client using the Client class map and set the client to non-blocking
				//	- If the password is correct, add the client to the epoll
				//	- If the password is incorrect, close the connection
			} else {
				std::cout << "Client trying to send a message" << std::endl;
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