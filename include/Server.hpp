/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/24 18:14:32 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "include.hpp"

bool running = true;

class Server
{
	public:
		Server(std::string port, std::string password);
		~Server();

		static void	handle_signal(int signal);
		int			start();
		int			loop();

		void		handle_message(int fd);	

	private:
		int						_socket_fd;
		int						_epoll_fd;
		int						_port;
		struct sockaddr_in		_addr;
		std::string				_password;
		std::map<int, Client>	_clients;
};