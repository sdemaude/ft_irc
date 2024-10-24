/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/24 11:43:14 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "include.hpp"

class Server
{
	public:
		Server(std::string port, std::string password);
		~Server();
		int	start();
		int	loop();

	private:
		//TODO? signal handler
		int						_socket_fd;
		int						_epoll_fd;
		int						_port;
		struct sockaddr_in		_addr;
		std::string				_password;
		std::map<int, Client>	_clients;
};