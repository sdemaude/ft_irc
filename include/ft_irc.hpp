/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:14:40 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/28 16:42:58 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// C headers
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

// C++ headers
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

// Custom headers
#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"

// Constants
#define BACKLOG             10
#define MAX_EVENTS          10
#define BUFFER_SIZE         10

// Error codes
#define ERR_NOSUCHNICK      std::string("401")
#define ERR_NOSUCHCHANNEL   std::string("403")
#define ERR_NOTREGISTERED   std::string("451")
#define ERR_NEEDMOREPARAMS  std::string("461")
#define ERR_UNKNOWNMODE     std::string("472")