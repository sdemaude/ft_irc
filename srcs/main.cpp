/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:55:18 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 20:02:51 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

int main(int argc, char **argv) {
	// Check if the number of arguments is correct
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}

	// Check if port is a number
	char *endPtr;	
	int port = std::strtod(argv[1], &endPtr);
	if (*endPtr != '\0') {
		std::cerr << "Error: Port must be a number" << std::endl;
		return EXIT_FAILURE;
	}

	// Check if port is a positive number under 65535
	if (port < 0 || port > 65535) {
		std::cerr << "Error: Port must be a positive number under 65535" << std::endl;
		return EXIT_FAILURE;
	}

	if (argv[2][0] == '\0') {
		std::cerr << "Error: Password cannot be empty" << std::endl;
		return EXIT_FAILURE;
	}

	// Create the server
	try {
		Server server(port, argv[2]);
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}