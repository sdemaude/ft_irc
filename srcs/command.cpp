/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:52:50 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 09:38:52 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

// You must be able to authenticate, set a nickname, a username, join a channel,
// send and receive private messages using your reference client.
// Then, you have to implement the commands that are specific to channel
// operators:
// ∗ KICK - Eject a client from the channel
// ∗ INVITE - Invite a client to a channel
// ∗ TOPIC - Change or view the channel topic
// ∗ MODE - Change the channel’s mode

// Command list:
// PASS - Set the server password
// PING - Send a ping to the server
// NICK - Set the nickname
// USER - Set the username
// JOIN - Join a channel
// PART - Leave a channel
// PRIVMSG - Send a private message
// QUIT - Disconnect from the server
// MODE - Change the mode of a channel
// KICK - Kick a user from a channel
// INVITE - Invite a user to a channel
// TOPIC - Set the topic of a channel
// MODE - Change the mode of a channel (I, T, K, O, L)


// Check if the client is registered before executing the command

void	Server::pass(Client &client, std::string &password) {
	// Check if the password is correct
	if (password == this->_password) {
		client.setRegistered(true);
		std::string response = ":" + this->getIpAdress() + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Client connected" << std::endl;
		return;
	}
	// Send an error message to the client if the password is incorrect 
	std::string response = ":" + this->getIpAdress() + " 464 " + client.getNickname() + " :Password incorrect\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	std::cout << "Password incorrect" << std::endl;
}

void	Server::ping(Client &client) {
	// send ping to the client
	// the client must send a pong
	// if the client doesn't send a pong, close the connection
	// timeout
}

void	Server::nick(Client &client, std::string &nickname) {
	// check if the nickname is already taken
	// if yes, add an underscore to the nickname and check again (repeat until the nickname is available)
	// set the nickname
	// send a response to the client
	std::map<int, Client>::iterator it = this->_clients.begin();
	while (it != this->_clients.end()) {
		if (it->second.getNickname() == nickname) {
			nickname += "_";
			it = this->_clients.begin();
		}
		it++;
	}
	client.setNickname(nickname);
	std::string response = ":" + this->getIpAdress() + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	std::cout << "Client connected" << std::endl;
}

void	Server::user(Client &client, std::string &username, std::string &hostname, std::string &realname) {
	//client.setUsername(username);
}

void	Server::join(Client &client, Channel &channel, std::string &password) {
	//channel.add_client(client);
}

void	Server::part(Client &client, Channel &channel) {
	//channel.remove_client(client);
}

void	Server::privmsg(Client &client, std::string message, Client &target) {
	//client.sendMessage(message);
}

void	Server::quit(Client &client) {
	// check if the client is in a channel
	// if yes, remove the client from the channel
	// if it is the last client, remove the channel	
	// close the connection
	// remove the client from the server
	// send a response to the client
}

void	Server::kick(Client &client, Channel &channel, Client &target) {
	//channel.remove_client(client);
}

void	Server::invite(Client &client, Channel &channel, Client &target) {
	//channel.add_client(client);
}

void	Server::topic(Client &client, Channel &channel, std::string &topic) {
	channel.setTopic(topic);
}

void	Server::mode(Client &client, Channel &channel, char mode, std::string &parameter) {
	// check if the client is an operator
	// if not, send a response to the client
	// if yes, call the right mode function
	if (channel.getUsers().at(client) != 1) {
		std::string response = ":" + client.getNickname() + " 482 " + client.getNickname() + " " + channel.getName() + " :You're not a channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		return;
	}
	/*switch (mode) {
		case 'I':
			mode_I(channel);
			break;
		case 'T':
			mode_T(channel, topic);
			break;
		case 'K':
			mode_K(channel, password);
			break;
		case 'O':
			mode_O(channel, client);
			break;
		case 'L':
			mode_L(channel, limit);
			break;
	}*/
}