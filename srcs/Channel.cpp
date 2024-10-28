/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 10:11:43 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 14:38:01 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : _invite_only(false), _limit(-1), _topic(""), _password(""), _name("") {
}

Channel::~Channel() {
}

// Send the message to all clients in the channel
void Channel::sendToAll(std::string &message) {
	std::map<Client, int>::iterator it = this->_users.begin();
	while (it != this->_users.end()) {
		send(it->first.getFd(), message.c_str(), message.size(), 0);
		it++;
	}
}

// Send the message to all clients in the channel except the client
void Channel::sendToOthers(std::string &message, Client &client) {
	std::map<Client, int>::iterator it = this->_users.begin();
	while (it != this->_users.end()) {
		if (it->first.getFd() != client.getFd()) {
			send(it->first.getFd(), message.c_str(), message.size(), 0);
		}
		it++;
	}
}

bool Channel::getInviteOnly() const {
    return this->_invite_only;
}

void Channel::setInviteOnly(bool invite_only) {
    this->_invite_only = invite_only;
}

std::string Channel::getTopic() const {
	return this->_topic;
}

void Channel::setTopic(std::string const &topic) {
	this->_topic = topic;
}

std::string Channel::getPassword() const {
	return this->_password;
}

void Channel::setPassword(std::string const &password) {
	this->_password = password;
}

int Channel::getLimit() const {
	return this->_limit;
}

void Channel::setLimit(int limit) {
	this->_limit = limit;
}

std::string Channel::getName() const {
	return this->_name;
}

void Channel::setName(std::string const &name) {
	this->_name = name;
}

std::map<Client, int> Channel::getUsers() const {
	return this->_users;
}

std::vector<Client> Channel::getWaitList() const {
	return this->_wait_list;
}

void Channel::add_client(Client &client) {
	// add the client to the channel
}

void Channel::remove_client(Client &client) {
	if (this->_users.find(client) != this->_users.end()) {
		this->_users.erase(client);
	}
}