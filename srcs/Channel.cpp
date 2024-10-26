/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 10:11:43 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 11:20:36 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : _invite_only(false), _limit(-1), _topic(""), _password(""), _name("") {
}

Channel::~Channel() {
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

//TODO? Setters for the users