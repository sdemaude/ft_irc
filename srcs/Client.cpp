/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccormon <ccormon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:40 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/26 13:39:14 by ccormon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd, std::string ip_addr) : _fd(fd), _ip_adrr(ip_addr) {
	this->_registered = false;
}

Client::~Client() {
	//close(this->_fd);
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

int	Client::getFd() const {
	return this->_fd;
}

bool	operator<(const Client &a, const Client &b) {
	return a.getFd() < b.getFd();
}
