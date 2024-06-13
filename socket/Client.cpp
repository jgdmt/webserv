/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 19:30:32 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client(Server &serv)
{
    socklen_t addr_len = sizeof(addr);
   _fd = accept(serv.getFdListen(), (sockaddr *)&addr ,&addr_len);
	if (_fd == -1)
	{
		// Print::error_print(ERROR, );
		throw std::runtime_error("Fail to accept new client: " + (std::string)strerror(errno));
	}
	char buffer[INET_ADDRSTRLEN];
	Print::print(INFO, serv, "New Connection on socket " + to_string(_fd) + " From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN));
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::runtime_error("Socket " + to_string(_fd) + " fcntl: " + (std::string)strerror(errno));
		// Print::print(ERROR, "Socket " + to_string(_fd) + " fcntl: " + (std::string)strerror(errno));
		close(_fd);
		// Print::print(ERROR, "Abort connection with client " + to_string(_fd));
		// return ;
	}
    
}

int Client::getFd(void) const
{
	return _fd;
}

void    Client::readRequest(void)
{
    Print::print(INFO, "cc");
}