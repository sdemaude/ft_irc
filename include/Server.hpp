/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 11:47:43 by ccormon          ###   ########.fr       */
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
		std::string	read_message(int fd);
		void		parse_message(std::map< int, Client >::iterator iter, std::string message);
		// Mode
		void		mode_I(Channel &channel);
		void		mode_T(Channel &channel, std::string &topic);
		void		mode_K(Channel &channel, std::string &password);
		void		mode_O(Channel &channel, Client &client);
		void		mode_L(Channel &channel, int limit);

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
