/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 14:44:46 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client
{
	public:
		Client(int fd, std::string ip_addr);
		~Client();
		//TODO? getter and setter for _fd and _ip_addr

	private:
		bool		_registered;
		int			_fd;
		std::string	_ip_adrr;
		// std::string	_nick;
		// std::string	_user;
		// std::string	_realname;

		//TODO?	_password _hostname	_servername _mode _channels
};