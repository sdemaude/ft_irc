/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:37 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/24 12:01:17 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "include.hpp"

class Client
{
	public:
		Client();
		~Client();
		//TODO? getter and setter for _fd and _ip_addr

	private:
		int			_fd;
		std::string	_ip_adrr;
		// std::string	_nick;
		// std::string	_user;
		// std::string	_realname;

		//TODO?	_password _hostname	_servername _mode _channels
};