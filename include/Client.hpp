/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 15:22:29 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client
{
	public:
		Client(int fd, std::string ip_addr);
		~Client();

		std::string	get_buffer();
		void		clear_buffer();
		void		set_buffer(std::string buffer);

		int			getFd() const;
		std::string	getNickname() const;
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
