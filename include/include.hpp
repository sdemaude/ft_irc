/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:14:40 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/24 18:04:27 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// C headers
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>

// C++ headers
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Custom headers
#include "../include/Client.hpp"
#include "../include/Server.hpp"

// Constants
#define BACKLOG 10
#define MAX_EVENTS 10