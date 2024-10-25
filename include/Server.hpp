/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 14:44:58 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client;

class Server
{
	public:
		Server(short port, std::string password);
		~Server();

		static void	handle_signal(int signal);
		int			init();
		int			loop();

		void		handle_connection();
		void		handle_message(int fd);	

	private:
		static bool				_running;
		int						_socket_fd;
		int						_epoll_fd;
		short					_port;
		struct sockaddr_in		_addr;
		std::string				_password;
		std::map<int, Client>	_clients;
};