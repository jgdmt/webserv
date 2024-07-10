/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 21:51:54 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "../parsing/Settings.hpp"

Client::Client(Server *serv, int id, Settings* settings): _settingsPtr(settings), _serverPtr(serv), _id(id)
{
    socklen_t addr_len = sizeof(_addr);
   _fd = accept(serv->getFdListen(), (sockaddr *)&_addr ,&addr_len);
	if (_fd == -1)
	{
		throw std::runtime_error("Fail to accept new client: " + (std::string)strerror(errno));
	}
	char buffer[INET_ADDRSTRLEN];
	Print::print(INFO, "New Connection with id " + to_string(_id) + "(socket " + to_string(_fd) + ") From " + (std::string)inet_ntop(AF_INET, &_addr, buffer, INET_ADDRSTRLEN), *_serverPtr);
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0 || fcntl(_fd, FD_CLOEXEC))
	{
		throw std::runtime_error("Socket " + to_string(_fd) + " fcntl: " + (std::string)strerror(errno));
		close(_fd);
	}
	_last_com = time(NULL);
}



Client::~Client(void)
{}


Client::Client(Client const &client): _serverPtr(client._serverPtr), _id(client._id)
{
	*this = client;
}

Client &Client::operator=(Client const &client)
{
	this->_addr = client._addr;
	this->_id = client._id;
	this->_settingsPtr = client._settingsPtr;
	this->_fd = client._fd;
	this->_serverPtr = client._serverPtr;
	this->_last_com = client._last_com;
	this->Response::_buffer = client.Response::_buffer;
	this->_cgiStatus = client._cgiStatus;
	// request var
	this->_i = client._i;
    this->Request::_buffer = client.Request::_buffer;
    this->_method = client._method;
    this->_uri = client._uri;
	this->_query = client._query;
    this->_host = client._host;
    this->_connection = client._connection;
    this->_contentType = client._contentType;
    this->_contentBoundary = client._contentBoundary;
    this->_body = client._body;
	this->_contentLength = client._contentLength;
	this->_accept = client._accept;
	this->_acceptEncoding = client._acceptEncoding;
	this->_state = client._state;
	this->_error = client._error;
	this->_headerStatus = client._headerStatus;
	return *this;
}

int Client::getFd(void) const
{
	return _fd;
}

int Client::getId(void) const
{
	return _id;
}

time_t Client::getLastCom(void) const
{
	return _last_com;
}

void    Client::setId(int id)
{
	_id = id;
}

sockaddr_in* Client::getAddr()
{
	return &_addr;
}

void    Client::readRequest(void)
{
	char buffer[READSIZE + 1];
	int i = _id + _settingsPtr->getServerSize();
	int readSize;
	Request::setClient(this);
	readSize = recv(_fd, buffer, READSIZE, O_NONBLOCK);
	if(readSize > 0)
		buffer[readSize] = 0; 
    switch (readSize)
	{
	case 0:
		_settingsPtr->closeClient(_id);
		return ;
	case -1:
		Print::print(ERROR, "Recv didn't work properly on client " + to_string<int>(_id) + " " + strerror(errno));
		_settingsPtr->closeClient(_id);
		return ;
	default:
		_last_com = time(NULL);
		// std::cout << "here\n";
		add(std::string(buffer, readSize));
		break;
	}
	switch(IsParsingOk())
	{
		case -3:
			error("413", "Content Too Large");
			_settingsPtr->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
		case -2:
			error("411", "Length Required");
			_settingsPtr->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
		case -1:
			error("400", "Bad Request");
			_settingsPtr->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
		case 0:
			break;
		case 1:
			init();
			if (!_cgiStatus)
				_settingsPtr->getFds()->at(i) = (pollfd){_fd, POLLOUT, 0};
			break;
	}

}

void    Client::sendResponse(void)
{
	int result;
	if(Response::_buffer.length() >= WRITESIZE)
		result = write(_fd, Response::_buffer.c_str(), WRITESIZE);
	else
		result = write(_fd, Response::_buffer.c_str(), Response::_buffer.length());
	if(result < 0)
			Print::print(ERROR, "Send failed for Client " + to_string<int>(_id));
	else if(result == 0 || (unsigned int)result == Response::_buffer.length())
	{
			Print::print(DEBUG, "Response sent to Client " + to_string<int>(_id) + ".", *_serverPtr);
			Request::clear();
			Response::_buffer.clear();
			_settingsPtr->getFds()->at(_id + _settingsPtr->getServerSize()) = (pollfd){_fd, POLLIN, 0};		
			_last_com = time(NULL);
			if(getConnection() != "keep-alive")
				_settingsPtr->closeClient(_id);
	}
	else
	{
		Response::cut(result);
		_last_com = time(NULL);
	}
}
