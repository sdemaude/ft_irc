/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:47:27 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 19:44:01 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client;

class Channel
{
	public:
		Channel(std::string const &name);
		Channel(const Channel &other);
		~Channel();

		void					sendToAll(std::string &message);
		void					sendToOthers(std::string &message, Client &client);
		// Getters and setters
		bool					getInviteOnly() const;
		void					setInviteOnly(bool invite_only);
		std::string				getTopic() const;
		void					setTopic(std::string const &topic);
		bool					getTopicLock() const;
		void					setTopicLock(bool topic_lock);
		std::string				getPassword() const;
		void					setPassword(std::string const &password);
		int						getLimit() const;
		void					setLimit(int limit);
		std::string				getName() const;
		void					setName(std::string const &name);
		std::map<Client *, int>	&getUsers();
		std::vector<Client *>	&getWaitList();

		void					add_waitlist(Client &client);

		void					add_client(Client &client);
		void					remove_client(Client &client);

	private:
		bool						_invite_only;	// false by default
		int							_limit; 		// -1 if no limit
		bool						_topic_lock;	// false by default
		std::string					_topic;			// Topic of the channel (can be empty if no topic)
		std::string					_password;		// Password of the channel (can be empty if no password)
		std::string					_name;			// #channel
		std::vector<Client *>		_wait_list;		// List of clients waiting to join the channel (invite only)
		std::map<Client *, int>		_users;			// List of users and their status (operator or not)
};
