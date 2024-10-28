/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:52:50 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 11:40:19 by sdemaude         ###   ########.fr       */
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

void	Server::ping(Client &client) {
	// Send a PONG response to the client
	std::string response = ":" + client.getId() + " PONG " + client.getNickname() + " :server\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
}

void	Server::pass(Client &client, std::string &password) {
	// Check if the password is correct
	if (password == this->_password) {
		client.setRegistered(true);
		//std::string response = ":" + this->getIpAdress() + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!\r\n";
		//send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Client connected" << std::endl;
	} else {
		// Send an error message to the client if the password is incorrect 
		std::string response = ":server 464 " + client.getNickname() + " :Password incorrect\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Password incorrect" << std::endl;
	}
}

// Check if the client is registered?
void	Server::nick(Client &client, std::string &nickname) {
	// Check if the client has already set a nickname
	if (client.getNickname() == "") {
		std::map<int, Client>::iterator it = this->_clients.begin();
		while (it != this->_clients.end()) {
		// If the nickname is already taken, add a _ to the nickname
		if (it->second.getNickname() == nickname) {
			nickname += "_";
			it = this->_clients.begin();
		}
		it++;
		}
		client.setNickname(nickname);
		std::string response = ":" + client.getId() + " NICK " + nickname + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Nickname set" << std::endl;
	} else { 
		// Check if the nickname is already taken and send an error message to the client if it is
		std::map<int, Client>::iterator it = this->_clients.begin();
		while (it != this->_clients.end()) {
			if (it->second.getNickname() == nickname) {
				std::string response = ":server 433 " + client.getNickname() + " " + nickname + " :Nickname is already in use\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
				std::cout << "Nickname already in use" << std::endl;
				return;
			}
			it++;
		}
		client.setNickname(nickname);
		std::string response = ":" + client.getId() + " NICK " + nickname + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Nickname changed" << std::endl;
	}
}

void	Server::user(Client &client, std::string &username, std::string &hostname, std::string &realname) {	
	// Check if the client is registered
	if (!client.getRegistered()) {
		// If not, send an error message to the client
		std::string response = ":server 451 " + client.getNickname() + " :You have not registered\r\n"; //TODO? check right format
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (client.getRegistration()) {
		// If the client is already registered, send an error message to the client
		std::string response = ":server 462 " + client.getNickname() + " :You may not reregister\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else {
		// Set the username, hostname and realname of the client
		client.setUsername(username);
		client.setHostname(hostname);
		client.setRealname(realname);
		client.setRegistration(true);
		std::string response = ":server 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!\r\n"; //TODO? check if its the right format
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "User registration completed" << std::endl;
	}
}


// For all the functions below, check if the client is registered before executing the command

//add the client to the channel if the password is correct and the channel is not invite only or the client is invited
void	Server::join(Client &client, Channel &channel, std::string &password) {
	//send an error message to the client if the password is incorrect or the channel is invite only and the client is not invited
	//send a response to the client if the client is added to the channel
	//send a response to all the clients in the channel if the client is added to the channel
	//send a response to the client if the client is already in the channel
	//send an error message to the client if the channel does not exist
	//send an error message to the client if the channel is full
	//send an error message to the client if the channel is invite only and the client is not invited
	//send an error message to the client if the channel has a password and the client does not provide it
	//send an error message to the client if the channel has a password and the client provides the wrong password
	if (channel.getUsers().find(client) != channel.getUsers().end()) {
		std::string response = ":server 443 " + client.getNickname() + " " + channel.getName() + " :is already in channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getPassword() != "") {
		if (password == channel.getPassword()) {
			channel.add_client(client);
			std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			channel.sendToAll(response); // send to all the clients in the channel??
		} else {
			std::string response = ":server 475 " + client.getNickname() + " " + channel.getName() + " :Bad channel key\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		}
	} else if (channel.getInviteOnly()) {
/*



		if (channel.getPassword() != "") {
			if (password == channel.getPassword()) {
				channel.add_client(client);
				std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
				std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
				channel.sendToAll(response); // send to all the clients in the channel??
			} else {
				std::string response = ":server 475 " + client.getNickname() + " " + channel.getName() + " :Bad channel key\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
			}
		} else {
			channel.add_client(client);
			std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
			channel.sendToAll(response); // send to all the clients in the channel??
			std::cout << "Client joined the channel" << std::endl;
		}*/
	} 
}

void	Server::part(Client &client, Channel &channel) {
	//channel.remove_client(client);
}

void	Server::privmsg(Client &client, std::string message, Client &target) {
	// Send the message to the target
	std::string response = ":" + client.getId() + " PRIVMSG " + target.getNickname() + " :" + message + "\r\n";
	send(target.getFd(), response.c_str(), response.size(), 0);
}

void	Server::quit(Client &client) {
	// Check if the client is in a channel
	if (client.getChannels().size() > 0) {
		std::map<std::string, Channel>::iterator it = client.getChannels().begin();
		while (it != client.getChannels().end()) {
			it->second.remove_client(client);
			// If the channel is empty, remove it
			if (it->second.getUsers().size() == 0) {
				this->_channels.erase(it->first);
			}
			it++;
		}
	}
	std::string response = ":" + client.getId() + " QUIT :Leaving\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	// Close the client socket and remove it from the clients map
	close(client.getFd());
	this->_clients.erase(client.getFd());
	std::cout << "Client disconnected" << std::endl;
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