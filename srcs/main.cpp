/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:55:18 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/23 17:29:00 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/include.hpp"

int main(int argc, char **argv) {

    // Check if the number of arguments is correct
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    // Create the server
    Server server(argv[1], argv[2]);

    return EXIT_SUCCESS;
}