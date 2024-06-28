/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/28 16:32:58 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "../parsing/Settings.hpp"

Client::Client(Server &serv, int id, Settings* settings): _settings(settings), _serv(serv), _cgi(&req, &_serv, settings), res(&req, &_serv, &_cgi), _id(id)
{
    socklen_t addr_len = sizeof(_addr);
   _fd = accept(serv.getFdListen(), (sockaddr *)&_addr ,&addr_len);
	if (_fd == -1)
	{
		throw std::runtime_error("Fail to accept new client: " + (std::string)strerror(errno));
	}
	char buffer[INET_ADDRSTRLEN];
	Print::print(INFO, "New Connection with id " + to_string(_id) + "(socket " + to_string(_fd) + ") From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), serv);
	// if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
	// {
	// 	throw std::runtime_error("Socket " + to_string(_fd) + " fcntl: " + (std::string)strerror(errno));
	// 	close(_fd);
	// }
	_last_com = time(NULL);
}

Client::~Client(void)
{
// close(_fd);
// char buffer[INET_ADDRSTRLEN];
// Print::print(INFO, "Connection closed on socket " + to_string(_fd) + " From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), _serv);
}


Client::Client(Client const &client): _serv(client._serv), _cgi(&req, &_serv, client._settings), res(&req, &_serv, &_cgi), _id(client._id)
{
	*this = client;
}

Client &Client::operator=(Client const &client)
{
	this->_addr = client._addr;
	this->_id = client._id;
	this->_fd = client._fd;
	this->_serv = client._serv;
	this->_last_com = client._last_com;
	this->req = client.req;
	this->res = client.res;
	this->_settings = client._settings;
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

void    Client::setId(int id)
{
	_id = id;
}

void    Client::readRequest(void)
{
	char buffer[READSIZE];
	int i = _id + _settings->getServers()->size();
	// bzero(buffer, READSIZE); // delete later
	
    switch (recv(_fd, buffer, READSIZE, MSG_DONTWAIT))
	{
	case 0:
		_settings->closeClient(_id);
		return ;
	case -1:
		Print::print(ERROR, "Recv didn't work properly on client " + to_string<int>(_id) + strerror(errno));
		_settings->closeClient(_id);
		return ;
	default:
		_last_com = time(NULL);
		req.add(buffer);
		break;
	}
	switch(req.IsParsingOk())
	{
		case -2:
			res.error("411", "Length Required");
			_settings->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};

			break;
		case -1:
			res.error("400", "Bad Request");
			_settings->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
		case 0:
			break;
		case 1:
			res.init();
			_settings->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
	}

}

void    Client::sendResponse(void)
{
	// Print::print(INFO, "ready", _serv);
	int result;
// // std::cout<< *(res.getRes()) << "\n";
	if(res.getRes()->size() >= WRITESIZE)
		result = send(_fd, res.getRes()->c_str(), WRITESIZE, MSG_DONTWAIT);
	else
		result = send(_fd, res.getRes()->c_str(), res.getRes()->size(), MSG_DONTWAIT);
	if(result < 0)
			Print::print(ERROR, "Send failed for Client " + to_string<int>(_id));
	else if(result == 0 || (unsigned int)result == res.getRes()->size())
	{
			Print::print(DEBUG, "Response sent to Client " + to_string<int>(_id) + ".", _serv);
			req.clear();
			res.getRes()->clear();
			_settings->getFds()->at(_id + _settings->getServers()->size()) = (pollfd){_fd, POLLIN, 0};		
			_last_com = time(NULL);
	}
	else
	{
		res.cut(result);
		_last_com = time(NULL);
	}
	


}
