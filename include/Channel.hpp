/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:47:27 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 14:45:24 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Channel
{
	public:
		Channel();
		~Channel();

		void					sendToAll(std::string &message);
		// Getters and setters
		bool					getInviteOnly() const;
		void					setInviteOnly(bool invite_only);
		std::string				getTopic() const;
		void					setTopic(std::string const &topic);
		std::string				getPassword() const;
		void					setPassword(std::string const &password);
		int						getLimit() const;
		void					setLimit(int limit);
		std::string				getName() const;
		void					setName(std::string const &name);
		std::map<Client, int>	getUsers() const;

	private:
		bool					_invite_only;	// false by default
		int						_limit; 		// -1 if no limit
		std::string				_topic;			// Topic of the channel (can be empty if no topic)
		std::string				_password;		// Password of the channel (can be empty if no password)
		std::string				_name;			// #channel
		std::map<Client, int>	_users;			// List of users and their status (operator or not)
};