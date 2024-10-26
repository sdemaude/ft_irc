/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 11:42:59 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool Server::_running = true;

Server::Server(short port, std::string password) : _port(port), _password(password) {
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
	std::cout << "[" << fd << "]   " << "Message received : " << message << std::endl;

	parse_message(this->_clients.find(fd), message);
}

std::string	Server::read_message(int fd) {
	char		buffer[BUFFER_SIZE];
	std::string	message = "";

	memset(&buffer, '\0', BUFFER_SIZE);
	while (1) {
		int	byte_read = recv(fd, buffer, BUFFER_SIZE, 0);

		if (byte_read <= 0)
			return "";

		buffer[byte_read] = '\0';
		message += buffer;

		if (byte_read < BUFFER_SIZE || buffer[BUFFER_SIZE - 1] == '\n')
			return message;
	}
}

/* Chaque message IRC peut consister en jusqu’à trois parties principales :
 - le préfixe (FACULTATIF),
 - la commande,
 - les paramètres de commande (maximum de quinze (15)).
Le préfixe, la commande, et tous les paramètres sont chacun séparés par un 
caractère ASCII espace (0x20).

La présence d’un préfixe est indiquée par un seul caractère ASCII 
deux-points (':', 0x3a) en tête, qui DOIT être le premier caractère du 
message lui-même. Il DOIT n’y avoir AUCUN trou (espace) entre le deux-points 
et le préfixe. Le préfixe est utilisé par les serveurs pour indiquer la 
vraie origine du message. Si le préfixe manque dans le message, il est 
supposé avoir été généré sur la connexion d’où il a été reçu. Les clients NE 
DEVRAIENT PAS utiliser un préfixe lors de l’envoi d’un message ; si ils en 
utilisent un, le seul préfixe valide est le pseudonyme enregistré associé au 
client.

La commande DOIT être une commande IRC valide ou un nombre de trois (3) 
chiffres représentés en texte ASCII.

Les messages IRC sont toujours des lignes de caractères terminées par une paire 
retour chariot-saut à la ligne (CR-LF, Carriage Return - Line Feed) et ces 
messages NE DEVRONT PAS excéder 512 caractères, en comptant tous les caractères 
y compris le CR-LF de queue. Donc, 510 caractères maximum sont alloués pour la 
commande et ses paramètres. Il n’y a aucune disposition pour la continuation 
des lignes du message. Voir à la section 6 des précisions sur les mises en 
œuvre actuelles.
 */
void	Server::parse_message(std::map< int, Client >::iterator iter, std::string message) {
	std::string	prefix = "";
	std::string	command = "";
	std::string	params = "";

	message = this->_clients[iter->first].get_buffer() + message;
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
