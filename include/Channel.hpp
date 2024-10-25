/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:47:27 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 18:04:15 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Channel
{
	public:
		Channel();
		~Channel();

    private:
		std::string					_name;
		std::string					_topic;
		std::string					_password;
		std::vector<std::string>	_users;
		// std::string				_mode;
		// std::string				_limit;
};