/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/18 17:22:07 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "../parsing/Settings.hpp"

Client::Client(Server &serv, int id): _id(id), _serv(serv)
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
// close(_fd);
// char buffer[INET_ADDRSTRLEN];
// Print::print(INFO, "Connection closed on socket " + to_string(_fd) + " From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), _serv);
}


Client::Client(Client const &client): _id(client._id), _serv(client._serv)
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

void    Client::readRequest(Settings *set)
{
	char buffer[READSIZE];
	// bzero(buffer, READSIZE); // delete later
    switch (recv(_fd, buffer, READSIZE, 0))
	{
	case 0:
		set->closeClient(_id);
		break;
	case -1:
		Print::print(ERROR, "Recv didn't work properly");
		set->closeClient(_id);
		break;
	default:
		_last_com = time(NULL);
		req.add(buffer);
		break;
	}
	std::cout << req.IsParsingOk() << "\n";
	switch(req.IsParsingOk())
	{
		case -2:
			res.error("411");
			break;
		case -1:
			res.error("400");
			break;
		case 0:
			break;
		case 1:
			res.init();
			break;
	}
}