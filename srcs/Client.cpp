/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:37:40 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 14:13:04 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd, std::string ip_addr) : _fd(fd), _ip_adrr(ip_addr) {
    this->_registered = false;
}

Client::~Client() {
    //close(this->_fd);
}