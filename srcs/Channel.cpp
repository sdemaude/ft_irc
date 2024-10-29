/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 10:11:43 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 09:31:29 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : _invite_only(false), _limit(-1), _topic(""), _password(""), _name("") {
	this->_invite_only = false;
	this->_limit = -1;
	this->_topic = "";
	this->_password = "";
	this->_name = "";
	this->_users = std::map<Client, int>();
	this->_wait_list = std::vector<Client>();
}

Channel::Channel(const Channel &other) {
	this->_invite_only = other._invite_only;
	this->_limit = other._limit;
	this->_topic = other._topic;
	this->_password = other._password;
	this->_name = other._name;
	this->_users = other._users;
	this->_wait_list = other._wait_list;
}

Channel::~Channel() {
}

// Send the message to all clients in the channel
void Channel::sendToAll(std::string &message) {
	std::map<Client, int>::iterator it = this->_users.begin();
	while (it != this->_users.end()) {
		send(it->first.getFd(), message.c_str(), message.size(), 0);
		std::cout << "Message sent to " << it->first.getNickname() << std::endl; // TODO remove
		it++;
	}
}

// Send the message to all clients in the channel except the client
void Channel::sendToOthers(std::string &message, Client &client) {
	std::map<Client, int>::iterator it = this->_users.begin();
	std::cout << "Size of users: " << this->_users.size() << std::endl; // TODO remove
	while (it != this->_users.end()) {
		if (it->first.getFd() != client.getFd()) {
			send(it->first.getFd(), message.c_str(), message.size(), 0);
			std::cout << "Message sent to " << it->first.getNickname() << std::endl; // TODO remove
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

void Channel::add_waitlist(Client &client) {
	this->_wait_list.push_back(client);
}

void Channel::add_client(Client &client) {
	if (this->_users.size() == 0)
		this->_users.insert(std::pair<Client, int>(client, 1));
	else
		this->_users.insert(std::pair<Client, int>(client, 0));
}

void Channel::remove_client(Client &client) {
	if (this->_users.find(client) != this->_users.end()) {
		this->_users.erase(client);
	}
}
