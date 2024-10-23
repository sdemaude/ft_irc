/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/23 17:27:41 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(std::string port, std::string password) : _password(password) {

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

	//check if password is empty ?

	//init server and create socket
}

Server::~Server() {
	//close socket ?
}