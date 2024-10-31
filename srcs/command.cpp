/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:52:50 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/30 10:06:28 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

// Command list:
// PASS - Check the server password
// PING - Send a ping to the server
// NICK - Set the nickname
// USER - Set the username
// JOIN - Join a channel
// PART - Leave a channel
// PRIVMSG - Send a private message
// QUIT - Disconnect from the server
// KICK - Kick a user from a channel
// INVITE - Invite a user to a channel
// TOPIC - Set the topic of a channel
// MODE - Change the mode of a channel (I, T, K, O, L)

void	Server::ping(Client &client) {
	// Send a PONG response to the client
	std::string response = ":server PONG server\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
}

void	Server::pass(Client &client, std::string &password) {
	// Check if the password is correct
	if (password == this->_password) {
		client.setRegistered(true);
		std::cout << "Client connected" << std::endl;
	} else {
		// Send an error message to the client if the password is incorrect 
		std::string response = ":server 464 " + client.getNickname() + " :Password incorrect\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "Password incorrect" << std::endl;
	}
}

void	Server::nick(Client &client, std::string &nickname) {
	// Check if the client is registered
	if (!client.getRegistered()) {
		// If not, send an error message to the client
		std::string response = ":server 451 " + client.getNickname() + " :You have not registered\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (!client.getRegistration()) {
		// If the client is registered but has not set a nickname, set the nickname
		std::map<int, Client *>::iterator it = this->_clients.begin();
		while (it != this->_clients.end()) {
			// If the nickname is already taken, add a _ to the nickname
			if (it->second->getNickname() == nickname) {
				nickname += "_";
				it = this->_clients.begin();
			}
			it++;
		}
		std::string response = ":" + client.getId() + " NICK :" + nickname + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		client.setNickname(nickname);
		std::cout << "Nickname set" << std::endl;
	} else { 
		// Check if the nickname is already taken and send an error message to the client if it is
		std::map<int, Client *>::iterator it = this->_clients.begin();
		while (it != this->_clients.end()) {
			if (it->second->getNickname() == nickname) {
				std::string response = ":server 433 " + client.getNickname() + " " + nickname + " :Nickname is already in use\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
				std::cout << "Nickname already in use" << std::endl;
				return;
			}
			it++;
		}
		// If the nickname is not taken, set the nickname and send a NICK message to all the clients
		std::string response = ":" + client.getId() + " NICK :" + nickname + "\r\n";
		std::map<int, Client *>::iterator it2 = this->_clients.begin();
		while (it2 != this->_clients.end()) {
			std::string response = ":" + client.getId() + " NICK :" + nickname + "\r\n";
			send(it2->second->getFd(), response.c_str(), response.size(), 0);
			it2++;
		}
		client.setNickname(nickname);
		std::cout << "Nickname changed" << std::endl;
	}
}

void	Server::user(Client &client, std::string &username, std::string &hostname, std::string &realname) {	
	// Check if the client is registered
	if (!client.getRegistered()) {
		// If not, send an error message to the client
		std::string response = ":server 451 " + client.getNickname() + " :You have not registered\r\n";
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
		std::string response = ":server 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		response = ":server 002 " + client.getNickname() + " :Your host is " + client.getHostname() + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		response = ":server 003 " + client.getNickname() + " :This server was created today\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		response = ":server 004 " + client.getNickname() + " :server\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		std::cout << "User registration completed" << std::endl;
	}
}

void	Server::join(Client &client, Channel &channel, std::string &password) {
	std::string channel_name = channel.getName();
	// Check if the channel is full
	if (channel.getLimit() != -1 && (int)channel.getUsers().size() >= channel.getLimit()) {
		std::string response = ":server 471 " + client.getNickname() + " " + channel.getName() + " :Cannot join channel (+l)\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getUsers().find(&client) != channel.getUsers().end()) {
		// Check if the client is already in the channel
		std::string response = ":server 443 " + channel.getName() + " " + client.getNickname() + " :is already in channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getInviteOnly()) {
		// Check if the channel is invite only
		std::vector<Client *>::iterator it = channel.getWaitList().begin();
		while (it != channel.getWaitList().end()) {
			if ((*it) == &client) {
				// Check if the client is invited, add it to the channel and remove it from the wait list
				channel.add_client(client);
				channel.getWaitList().erase(it);
				client.addChannel(channel_name);
				std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
				channel.sendToAll(response);
				std::cout << "Client joined the channel" << std::endl;
				return;
			}
			it++;
		}
		// If the client is not invited, add it to the wait list
		std::string response = ":server 473 " + client.getNickname() + " " + channel.getName() + " :Cannot join channel (+i)\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getPassword() != "") {
		// Check if the channel has a password
		if (!password.empty() && password == channel.getPassword()) {
			channel.add_client(client);
			client.addChannel(channel_name);
			//client.getChannels().insert(std::pair<std::string, Channel>(channel.getName(), channel));
			std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
			channel.sendToAll(response);
		} else {
			std::string response = ":server 475 " + client.getNickname() + " " + channel.getName() + " :Bad channel key\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		}
	} else {
		// If the channel is not invite only and does not have a password, add the client to the channel
		channel.add_client(client);
		client.addChannel(channel_name);
		std::string response = ":" + client.getId() + " JOIN " + channel.getName() + "\r\n";
		channel.sendToAll(response);
		std::cout << "Client joined the channel" << std::endl;
	}
}

void	Server::part(Client &client, Channel &channel) {
	std::string channel_name = channel.getName();
	// Check if the client is in the channel
	if (channel.getUsers().find(&client) == channel.getUsers().end()) {
		std::string response = ":server 442 " + client.getNickname() + " " + channel.getName() + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else {
		// If the client is in the channel, remove it
		channel.remove_client(client);
		client.removeChannel(channel_name);
		std::string response = ":" + client.getId() + " PART " + channel.getName() + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		channel.sendToAll(response);
		std::cout << "Client left the channel" << std::endl;
		if (channel.getUsers().size() == 0) {
			// If the channel is empty, remove it
			delete &channel;
			this->_channels.erase(channel_name);
		}
	}
}

void	Server::privmsg(Client &client, std::string &target, std::string &message) {
	// Check if the target is a channel or a client
	if (target[0] == '#') {
		// If the target is a channel, send the message to all the clients in the channel
		if (this->_channels.find(target) == this->_channels.end()) {
			// If the channel does not exist, send an error message
			std::string response = ":server 404 " + client.getNickname() + " " + target + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		} else if (this->_channels[target]->getUsers().find(&client) == this->_channels[target]->getUsers().end()) {
			// If the client is not in the channel, send an error message
			std::string response = ":server 442 " + client.getNickname() + " " + target + " :You're not on that channel\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		} else {
			// Send the message to all the clients in the channel
			std::string response = ":" + client.getId() + " PRIVMSG " + target + " :" + message + "\r\n";
			this->_channels[target]->sendToOthers(response, client);
		}
	} else {
		// If the target is a client, send the message to the target client
		std::map<int, Client *>::iterator it = this->_clients.begin();
		while (it != this->_clients.end()) {
			if (it->second->getNickname() == target) {
				std::string response = ":" + client.getId() + " PRIVMSG " + target + " :" + message + "\r\n";
				std::cout << "PRIVMSG response : " << response << std::endl;
				send(it->second->getFd(), response.c_str(), response.size(), 0);
				return;
			}
			it++;
		}
		// If the target client does not exist, send an error message to the client
		std::string response = ":server 401 " + client.getNickname() + " " + target + " :No such nick/channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	}
}

void	Server::quit(Client &client) {
	// Check if the client is in a channel
	if (client.getChannels().size() > 0) {
		while (client.getChannels().size() > 0) {
			// Send a QUIT message to all the clients in the channel
			std::string response = ":" + client.getId() + " QUIT :Leaving\r\n";
			this->_channels[client.getChannels()[0]]->sendToAll(response);
			this->_channels[client.getChannels()[0]]->remove_client(client);
			// If the channel is empty, remove it
			if (this->_channels[client.getChannels()[0]]->getUsers().size() == 0) {
				delete this->_channels[client.getChannels()[0]];
				this->_channels.erase(client.getChannels()[0]);
			} else {
				client.removeChannel(client.getChannels()[0]);
			}
		}
	}
	// Close the client socket and remove it from the clients map
	close(client.getFd());
	std::cout << "Client disconnected" << std::endl;
}

void	Server::kick(Client &client, Channel &channel, Client &target) {
	// Check if the client is an operator
	std::map<Client *, int>::iterator it = channel.getUsers().find(&client);
	if (it == channel.getUsers().end()) {
		std::string response = ":server 442 " + client.getNickname() + " " + channel.getName() + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (it->second == 0) {
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getUsers().find(&target) == channel.getUsers().end()) {
		// Check if the client is in the channel
		std::string response = ":server 441 " + client.getNickname() + " " + target.getNickname() + " " + channel.getName() + " :They aren't on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else {
		// If the client is in the channel, remove it
		channel.remove_client(target);
		std::string response = ":" + client.getId() + " KICK " + channel.getName() + " " + target.getNickname() + " :Kicked by " + client.getNickname() + "\r\n";
		send(target.getFd(), response.c_str(), response.size(), 0);
		channel.sendToAll(response);
	}
}

void	Server::invite(Client &client, Channel &channel, Client &target) {
	// Check if the client is an operator
	std::map<Client *, int>::iterator it = channel.getUsers().find(&client);
	if (it == channel.getUsers().end()) {
		std::string response = ":server 442 " + client.getNickname() + " " + channel.getName() + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (it->second == 0) {
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (!channel.getInviteOnly()) {
		// Check if the channel is invite only
		std::string response = ":server 518 " + client.getNickname() + " " + channel.getName() + " :Channel is not invite only\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (channel.getUsers().find(&target) != channel.getUsers().end()) {
		// Check if the target is already in the channel
		std::string response = ":server 443 " + client.getNickname() + " " + target.getNickname() + " " + channel.getName() + " :is already in channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else {
		// If the target is not in the channel, invite it to the channel and add it to the wait list
		std::string response = ":" + client.getId() + " INVITE " + target.getNickname() + " " + channel.getName() + "\r\n";
		send(target.getFd(), response.c_str(), response.size(), 0);
		std::string	response2 = ":server 341 " + client.getNickname() + " " + target.getNickname() + " " + channel.getName() + " :Invited " + target.getNickname() + " to " + channel.getName() + "\r\n";
		send(client.getFd(), response2.c_str(), response2.size(), 0);
		channel.add_waitlist(target);
		std::cout << "Client invited to the channel " << channel.getName() << std::endl;
	}
}

void	Server::topic(Client &client, Channel &channel, std::string &topic, bool view_topic) {
	// Check if the client is an operator
	std::map<Client *, int>::iterator it = channel.getUsers().find(&client);
	if (it == channel.getUsers().end()) {
		std::string response = ":server 442 " + client.getNickname() + " " + channel.getName() + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (it->second == 0) {
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (view_topic) {
		// If the topic is empty, send the topic to the client
		if (channel.getTopic() == "") {
			std::string response = ":server 331 " + client.getNickname() + " " + channel.getName() + " :No topic is set\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		} else {
			std::string response = ":server 332 " + client.getNickname() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n";
			send(client.getFd(), response.c_str(), response.size(), 0);
		}
	} else if (channel.getTopicLock()) {
		// Check if the topic is locked
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (topic == "") {
		// If the topic is empty, remove the topic
		channel.setTopic(topic);
		std::string response = ":" + client.getId() + " TOPIC " + channel.getName() + " :" + topic + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		channel.sendToOthers(response, client);
	} else {
		// If the topic is not empty, set the topic
		channel.setTopic(topic);
		std::string response = ":" + client.getId() + " TOPIC " + channel.getName() + " :" + topic + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
		channel.sendToOthers(response, client);
	}
}

void	Server::mode(Client &client, Channel &channel, char mode, std::string &parameter, bool set) {
	// Check if the client is an operator using the value of the map at the client key
	std::map<Client *, int>::iterator it = channel.getUsers().find(&client);
	if (it == channel.getUsers().end()) {
		std::string response = ":server 442 " + client.getNickname() + " " + channel.getName() + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else if (it->second == 0) {
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	} else {
		// Check the mode and execute the command
		switch (mode) {
			case 'i':
				mode_I(client, channel);
				break;
			case 't':
				mode_T(client, channel, parameter);
				break;
			case 'k':
				mode_K(client, channel, parameter);
				break;
			case 'o': {
				// Convert the parameter to an int and check if the client exists
				int target_fd = this->getFdByNickname(parameter);
				std::map<int, Client *>::iterator it = this->_clients.find(target_fd);
				while (it != this->_clients.end()) {
					if (it->first == target_fd) {
						mode_O(client, channel, *it->second, set);
						break;
					}
					it++;
				}
				std::string response = ":server 401 " + client.getNickname() + " " + parameter + " :No such nick/channel\r\n";
				send(client.getFd(), response.c_str(), response.size(), 0);
				break;
			}
			case 'l': {
				// Convert the parameter to an int
				if (parameter == "") {
					mode_L(client, channel, -1);
				} else {
					int limit = strtod(parameter.c_str(), NULL);
					mode_L(client, channel, limit);
				}
				break;
			}
		}
	}
}
