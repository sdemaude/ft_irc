/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 11:47:15 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client;
class Channel;

class Server {
	public:
		Server(short port, std::string &password);
		~Server();

		static void	handle_signal(int signal);
		int			init();
		int			loop();
		void		handle_connection();
		void		handle_message(int fd);
		std::string	read_message(int fd);
		void		parse_message(Client &client, std::string message);
		void		parse_command(Client &client, std::string prefix, std::string command, std::string params);
		// Command
		void		pass(Client &client, std::string &password);
		void		ping(Client &client);
		void		nick(Client &client, std::string &nickname);
		void		user(Client &client, std::string &username, std::string &hostname, std::string &realname);
		void		join(Client &client, Channel &channel, std::string &password);
		void		part(Client &client, Channel &channel);
		void		privmsg(Client &client, std::string message, Client &target);
		void		quit(Client &client);
		void		kick(Client &client, Channel &channel,  Client &target);
		void		invite(Client &client, Channel &channel, Client &target);
		void		topic(Client &clilent, Channel &channel, std::string &topic);
		void		mode(Client &client, Channel &channel, char mode, std::string &parameter);
		// Mode
		void		mode_I(Channel &channel, Client &client);
		void		mode_T(Channel &channel, Client &client, std::string &topic);
		void		mode_K(Channel &channel, Client &client, std::string &password);
		void		mode_O(Channel &channel, Client &client, Client &target);
		void		mode_L(Channel &channel, Client &client, int limit);
		
		//std::string getIpAdress() const;

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
