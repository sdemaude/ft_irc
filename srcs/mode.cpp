/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemaude <sdemaude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:55:14 by sdemaude          #+#    #+#             */
/*   Updated: 2024/10/25 15:45:58 by sdemaude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// Check before if the user is an operator in the MODE command

void    Server::mode_I() {
    // Check if the channel is already invite-only
    // If it is, remove the invite-only
    // If it is not, add the invite-only
}

void   Server::mode_T() {
    // Check if the channel is already topic-locked
    // If it is, remove the topic-locked
    // If it is not, add the topic-locked
}

void    Server::mode_K() {
    // Check if there is already a password
    // If there is a password, change it
    // If there is no password, add one
    // If the password is empty, remove it
}

void    Server::mode_O() {
    // Check if the user is an operator
    // If it is, remove the operator
    // If it is not, add the operator
}

void    Server::mode_L() {
    // Check if there is already a limit
    // If there is a limit, change it
    // If there is no limit, add one
    // If the limit is empty, remove it
}
