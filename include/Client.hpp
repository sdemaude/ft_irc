/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 14:10:21 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client
{
	public:
		Client(int fd, std::string ip_addr);
		~Client();

		int			getFd() const;
		std::string	getNickname() const;
		std::string	getBuffer();
		void		setRegistered(bool registered);

	private:
		bool		_registered; // false by default
		int			_fd;
		std::string	_ip_adrr;
		std::string	_buffer;
		std::string	_nick;
		std::string	_user;
		//_hostname	_channels
};

bool operator<(const Client &a, const Client &b);
