/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:40 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/29 19:55:02 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd, std::string ip_addr) : _fd(fd), _ip_addr(ip_addr) {
	this->_registered = false;
	this->_registration = false;
	this->_nickname = "";
	this->_username = "";
	this->_hostname = "";
	this->_realname = "";
	this->_buffer = "";
}

Client::Client(const Client &other) {
	this->_fd = other._fd;
	this->_ip_addr = other._ip_addr;
	this->_buffer = other._buffer;
	this->_nickname = other._nickname;
	this->_username = other._username;
	this->_hostname = other._hostname;
	this->_realname = other._realname;
	this->_registered = other._registered;
	this->_registration = other._registration;
}

Client::~Client() {
	close(this->_fd);
}

std::string	Client::get_buffer() {
	return this->_buffer;
}

void	Client::clear_buffer() {
	this->_buffer.clear();
}

void	Client::set_buffer(std::string buffer) {
	this->_buffer = buffer;
}

int Client::getFd() const {
    return (this->_fd);
}

std::string Client::getId() const {
	std::string id = this->_nickname + "!" + this->_username + "@" + this->_ip_addr;
	return (id);
}

std::string Client::getNickname() const {
    return (this->_nickname);
}

void Client::setNickname(std::string &nickname) {
	this->_nickname = nickname;
}

std::string Client::getUsername() const {
	return (this->_username);
}

void Client::setUsername(std::string &user) {
	this->_username = user;
}

std::string Client::getHostname() const {
	return (this->_hostname);
}

void Client::setHostname(std::string &hostname) {
	this->_hostname = hostname;
}

std::string Client::getRealname() const {
	return (this->_realname);
}

void Client::setRealname(std::string &realname) {
	this->_realname = realname;
}

bool Client::getRegistered() const {
	return (this->_registered);
}

void Client::setRegistered(bool registered) {
    this->_registered = registered;
}

bool Client::getRegistration() const {
	return (this->_registration);
}

void Client::setRegistration(bool registration) {
	this->_registration = registration;
}

void Client::addChannel(std::string &channel) {
	this->_channels.push_back(channel);
}

void Client::removeChannel(std::string &channel) {
	std::vector<std::string>::iterator it = this->_channels.begin();
	while (it != this->_channels.end()) {
		if (*it == channel) {
			this->_channels.erase(it);
			break;
		}
		it++;
	}
}

std::vector<std::string> &Client::getChannels() {
	return (this->_channels);
}

bool operator<(const Client &a, const Client &b) {
    return (a.getFd() < b.getFd());
}
