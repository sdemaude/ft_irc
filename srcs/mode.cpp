/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:55:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 19:02:18 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void	Server::mode_I(Client &client, Channel &channel) {
	// Check if the channel is invite only
	if (channel.getInviteOnly()) {
		channel.setInviteOnly(false);
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " -i\r\n";
		channel.sendToAll(response);
	} else {
		// If the channel is not invite only, set it to invite only
		channel.setInviteOnly(true);
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " +i\r\n";
		channel.sendToAll(response);
	}
}

void	Server::mode_T(Client &client, Channel &channel, std::string &topic) {
	// Check if the topic is locked
	if (topic == "") {
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " -t\r\n";
		channel.sendToAll(response);
		channel.setTopicLock(false);
	} else {
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " +t\r\n";
		channel.sendToAll(response);
		channel.setTopicLock(true);
	}
}

void	Server::mode_K(Client &client, Channel &channel, std::string &password) {
	// Set the password of the channel
	channel.setPassword(password);
	if (password != "") {
		std::cout << "password: " << password << std::endl;
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " +k " + password + "\r\n";
		channel.sendToAll(response);
	} else {
		// If there is no password parameter, remove the password
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " -k\r\n";
		channel.sendToAll(response);
	}
}

void	Server::mode_O(Client &client, Channel &channel, Client &target, bool set) {
	// Check if the target is already an operator in the channel
	if (!set && channel.getUsers().at(&target) == 1) {
		channel.getUsers().at(&target) = 0;
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " -o " + target.getNickname() + "\r\n";
		channel.sendToAll(response);
	} else if (set && channel.getUsers().at(&target) == 0) {
		// If the target is not an operator in the channel, add it
		channel.getUsers().at(&target) = 1;
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " +o " + target.getNickname() + "\r\n";
		channel.sendToAll(response);
	}
}

void	Server::mode_L(Client &client, Channel &channel, int limit) {
	// Set the limit of the channel
	channel.setLimit(limit);
	std::cout << "limit: " << limit << std::endl;
	// If there is a limit parameter add it to the channel
	if (limit != -1) {
		std::ostringstream oss;
		oss << limit;
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " +l " + oss.str() + "\r\n";
		channel.sendToAll(response);
	} else {
		// If there is no limit parameter, remove it from the channel
		std::string response = ":" + client.getId() + " MODE " + channel.getName() + " -l\r\n";
		channel.sendToAll(response);
	}
}
