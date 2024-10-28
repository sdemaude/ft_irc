/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:41:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 15:57:44 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client;
class Channel;

class Server {
	public:
		Server(int port, std::string password);
		~Server();

		static void	handle_signal(int signal);
		int			init();
		int			loop();
		void		handle_connection();
		void		handle_message(int fd);
		std::string	read_message(int fd);
		void		parse_message(Client &client, std::string message);
		void		parse_command(Client &client, std::string prefix, std::string command, std::string params);
		int			getFdByNickname(std::string &nickname);
		// Command
		void		pass(Client &client, std::string &password);
		void		ping(Client &client);
		void		nick(Client &client, std::string &nickname);
		void		user(Client &client, std::string &username, std::string &hostname, std::string &realname);
		void		join(Client &client, Channel &channel, std::string &password);
		void		part(Client &client, Channel &channel);
		void		privmsg(Client &client, std::string &target, std::string &message);
		void		quit(Client &client);
		void		kick(Client &client, Channel &channel,  Client &target);
		void		invite(Client &client, Channel &channel, Client &target);
		void		topic(Client &client, Channel &channel, std::string &topic);
		void		mode(Client &client, Channel &channel, char mode, std::string &parameter);
		// Mode
		void		mode_I(Client &client, Channel &channel);
		void		mode_T(Client &client, Channel &channel, std::string &topic);
		void		mode_K(Client &client, Channel &channel, std::string &password);
		void		mode_O(Client &client, Channel &channel, Client &target);
		void		mode_L(Client &client, Channel &channel, int limit);
		
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
