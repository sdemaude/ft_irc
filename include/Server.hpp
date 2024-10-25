/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 15:35:34 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client;
class Channel;

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
		// Mode
		void		mode_I();
		void		mode_T();
		void		mode_K();
		void		mode_O();
		void		mode_L();

	private:
		static bool						_running;
		int								_socket_fd;
		int								_epoll_fd;
		short							_port;
		struct sockaddr_in				_addr;
		std::string						_password;
		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;
};