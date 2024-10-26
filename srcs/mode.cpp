/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:55:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 14:52:44 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// Check before if the user is an operator in the MODE command
// Send the response to the user

void    Server::mode_I(Channel &channel, Client &client) {
	if (channel.getInviteOnly()) {
		channel.setInviteOnly(false);
		std::string response = ":server MODE " + channel.getName() + " -i\r\n";
		//send(client.getFd(), response.c_str(), response.size(), 0); // is there a way to do it with all clients in the channel?
		channel.sendToAll(response);
	} else {
		channel.setInviteOnly(true);
		std::string response = ":server MODE " + channel.getName() + " +i\r\n";
		channel.sendToAll(response);
	}
}

void   Server::mode_T(Channel &channel, Client &client, std::string &topic) {
	if (channel.getTopic() == "") {
		channel.setTopic(topic);
		std::string response = ":server MODE " + channel.getName() + " +t " + topic + "\r\n";
		channel.sendToAll(response);
	} else {
		channel.setTopic("");
		std::string response = ":server MODE " + channel.getName() + " -t\r\n";
		channel.sendToAll(response);
	}
}

// If there is a password, change it
// If there is no password, add one
// If the password is empty, remove it
void    Server::mode_K(Channel &channel, Client &client, std::string &password) {
	channel.setPassword(password);
	if (password != "") {
		std::string response = ":server MODE " + channel.getName() + " +k " + password + "\r\n";
		channel.sendToAll(response);
	} else {
		std::string response = ":server MODE " + channel.getName() + " -k\r\n";
		channel.sendToAll(response);
	}
}

void    Server::mode_O(Channel &channel, Client &client, Client &target) {
	if (channel.getUsers().find(client) != channel.getUsers().end()) {
		channel.getUsers().at(client) = 0;
		std::string response = ":server MODE " + channel.getName() + " -o " + target.getNickname() + "\r\n";
		channel.sendToAll(response);
	} else {
		channel.getUsers().insert(std::pair<Client, int>(client, 1));
		std::string response = ":server MODE " + channel.getName() + " +o " + target.getNickname() + "\r\n";
		channel.sendToAll(response);
	}
}

// If there is a limit, change it
// If there is no limit, add one
// If the limit is empty, remove it
void    Server::mode_L(Channel &channel, Client &client, int limit) {
	channel.setLimit(limit);
	if (limit != -1) {
		std::string response = ":server MODE " + channel.getName() + " +l " + std::to_string(limit) + "\r\n";
		channel.sendToAll(response);
	} else {
		std::string response = ":server MODE " + channel.getName() + " -l\r\n";
		channel.sendToAll(response);
	}
}
