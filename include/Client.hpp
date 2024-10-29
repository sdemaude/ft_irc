/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 19:54:03 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Channel;

class Client
{
	public:
		Client(int fd, std::string ip_addr);
		Client(const Client &other);
		~Client();

		std::string						get_buffer();
		void							set_buffer(std::string buffer);
		void							clear_buffer();

		int								getFd() const;
		std::string						getId() const;
		std::string						getNickname() const;
		void							setNickname(std::string &nickname);
		std::string						getUsername() const;
		void 							setUsername(std::string &user);
		std::string						getHostname() const;
		void							setHostname(std::string &hostname);
		std::string						getRealname() const;
		void							setRealname(std::string &realname);
		bool							getRegistered() const;
		void							setRegistered(bool registered);
		bool							getRegistration() const;
		void							setRegistration(bool registration);

		void							addChannel(std::string &channel);
		void							removeChannel(std::string &channel);
		std::vector<std::string>		&getChannels();

	private:
		bool							_registered; // false by default (for the password)
		bool							_registration; // false by default (for the user info)
		int								_fd;
		std::string						_ip_addr;
		std::string						_buffer;
		std::string						_nickname;
		std::string						_username;
		std::string 					_hostname;
		std::string						_realname;
		std::vector<std::string>		_channels;
};

bool operator<(const Client &a, const Client &b);
