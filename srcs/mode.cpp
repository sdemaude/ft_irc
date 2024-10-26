/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:55:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 11:37:44 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// Check before if the user is an operator in the MODE command
// Send the response to the user

void    Server::mode_I(Channel &channel) {
	if (channel.getInviteOnly())
		channel.setInviteOnly(false);
	else
		channel.setInviteOnly(true);
}

void   Server::mode_T(Channel &channel, std::string &topic) {
	if (channel.getTopic() == "")
		channel.setTopic(topic);
	else
		channel.setTopic("");
}

// If there is a password, change it
// If there is no password, add one
// If the password is empty, remove it
void    Server::mode_K(Channel &channel, std::string &password) {
	channel.setPassword(password);
}

// If it is, remove the operator
// If it is not, add the operator
void    Server::mode_O(Channel &channel, Client &client) {
	if (channel.getUsers().find(client) != channel.getUsers().end())
		channel.getUsers().at(client) = 0;
	else
		channel.getUsers().insert(std::pair<Client, int>(client, 1));
}

// If there is a limit, change it
// If there is no limit, add one
// If the limit is empty, remove it
void    Server::mode_L(Channel &channel, int limit) {
	channel.setLimit(limit);
}
