/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/14 11:21:18 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client(Server &serv): _serv(serv)
{
    socklen_t addr_len = sizeof(_addr);
   _fd = accept(serv.getFdListen(), (sockaddr *)&_addr ,&addr_len);
	if (_fd == -1)
	{
		throw std::runtime_error("Fail to accept new client: " + (std::string)strerror(errno));
	}
	char buffer[INET_ADDRSTRLEN];
	Print::print(INFO, "New Connection on socket " + to_string(_fd) + " From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), serv);
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::runtime_error("Socket " + to_string(_fd) + " fcntl: " + (std::string)strerror(errno));
		close(_fd);
	}
	_last_com = time(NULL);
    
}

Client::~Client(void)
{
	close(_fd);
	char buffer[INET_ADDRSTRLEN];
	Print::print(INFO, "Connection closed on socket " + to_string(_fd) + " From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), _serv);
}

Client::Client(Client const &client): _serv(client._serv)
{
	*this = client;
}

Client &Client::operator=(Client const &client)
{
	this->_addr = client._addr;
	this->_fd = client._fd;
	this->_serv = client._serv;
	this->_last_com = client._last_com;
	return *this;
}

int Client::getFd(void) const
{
	return _fd;
}

time_t Client::getLastCom(void) const
{
	return _last_com;
}

Server &Client::getServer(void)
{
	return _serv;
}

void    Client::readRequest(void)
{
    Print::print(INFO, "cc");
}