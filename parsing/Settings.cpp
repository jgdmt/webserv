/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:00:33 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 20:19:20 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include "../Print.hpp"
#include <iterator>

typedef std::string::iterator it;

std::vector<pollfd> *Settings::getFds()
{
	return &_fds;
}

std::vector<Client> *Settings::getClients(void)
{
	return &_clients;
}

std::vector<CGI> *Settings::getCgi()
{
	return &_cgis;
}

static std::string createContent(std::string const& fileName)
{
	std::ifstream ifs(fileName.c_str());
	std::string	line;
	std::string content;

	if (!ifs || !ifs.is_open())
	{
		Print::print(ERROR, "File error.");
		ifs.open("default/webserv.conf");
	}
	while (ifs.good())
	{
		std::getline(ifs, line);
		content.append(line);
	}
	ifs.close();
	std::replace(content.begin(), content.end(), '\t', ' ');
	return (content);
}

static it	find_end(std::string const& content, it i)
{
	int nb = 1;
	while (i != content.end())
	{
		if (*i == '{')
			nb++;
		if (*i == '}')
			nb--;
		if (nb == 0)
			break;
		i++;
	}
	if (nb > 0)
		Print::print(CRASH, "Bracket error: } not found");
	return (i);
}

void Settings::parseServer(std::string const& content, it& name, it& start, it& end)
{
	int len = Route::find_len(content, name, '{', true);
	
	if (name == start || content.substr(name - content.begin(), len) != "server")
		Print::print(CRASH, content.substr(name - content.begin(), len) + " is an unknown key");
	Server server(this->_servers.size() + 1);
	server.parse(content, start, end);
	this->_servers.push_back(server);
}

void Settings::parse(std::string const &fileName)
{
	std::string content = createContent(fileName);
	it	start = content.begin();
	it	name;

	if (start == content.end())
		Print::print(CRASH, "Parsing: file is empty");
	while (start != content.end())
	{
		name = start;
		while (start != content.end() && *start != '{')
		{
			if (*start == '}')
				Print::print(CRASH, "Bracket error: } has no {");
			if (*name == ' ')
				name++;
			start++;
		}
		it end = find_end(content, start + 1);
		if (end == content.end())
			return ;
		parseServer(content, name, start, end);
		start++;
	}
	Print::print(SUCCESS, "Parsing finished successfully");
}

std::vector<Server> *Settings::getServers(void)
{
	return &_servers;
}

void Settings::setup(void)
{
	char buffer[INET_ADDRSTRLEN];
	for(int i = 0; i < (int)_servers.size(); i++)
	{
		_servers[i].setup();
		pollfd tmp = {_servers[i].getFdListen(), POLLIN, 0};
		_fds.push_back(tmp);
		Print::print(INFO, "Server started! Listen at " + (std::string)inet_ntop(AF_INET, &_servers[i].getHost(), buffer, INET_ADDRSTRLEN) + ":" + to_string<uint16_t>(_servers[i].getPort()) + ".", _servers[i]);
	}
	Print::print(SUCCESS,"All servers succesfully started !");
}

void Settings::run(void)
{
	while(1)
	{
		if(poll(_fds.data(), _fds.size(), 30) == -1)
			Print::print(CRASH, "Poll: " + (std::string)strerror(errno));
		for(unsigned int i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents == POLLERR || _fds[i].revents == POLLHUP || _fds[i].revents == POLLNVAL)
			{
				Print::print(ERROR, "Poll: at client id " + to_string<int>(i - 1) + ": " +  (std::string)strerror(errno));
				if(i >= _servers.size() && _clients.size() > i)
					closeClient(i - _servers.size());
				else if (i > _servers.size() + _clients.size() && i < _cgis.size() > i)
					_cgis[i - (_servers.size() + _clients.size())].closeCGI(i);
				else
					exit(1);
			}
			else if(_fds[i].revents & POLLIN && _servers.size() > i)
				addClient(_servers[i]);
			else if (_fds[i].revents & POLLIN && _clients.size() > (i - _servers.size())) // && _clients[i - _servers.size()].getFd() == _fds[i].fd
				_clients[i - _servers.size()].readRequest();
			else if (_fds[i].revents & POLLOUT && _clients.size() > (i - _servers.size())) // && _clients[i - _servers.size()].getFd() == _fds[i].fd
				_clients[i - _servers.size()].sendResponse();
			else if (_fds[i].revents & POLLIN && _clients.size() + _servers.size() <= i)
				_cgis[i - _servers.size() - _clients.size()].body(i);
		}
		checkTimeout();
	}
}

void Settings::checkTimeout(void)
{
	for(int i = 0; i < (int)_clients.size(); i++)
	{
		if((time(NULL) - _clients[i].getLastCom()) > TIMEOUT)
		{
			Print::print(DEBUG, "TIMEOUT for client on socket " + to_string(_clients[i].getFd()), *_clients[i]._serverPtr);
			closeClient(i);
			i--;
		}
	}
	for(int i = 0; i < (int)_cgis.size(); i++)
	{
		if((time(NULL) - _cgis[i].getStartTime()) > TIMEOUT / 2)
		{
			Print::print(DEBUG, "TIMEOUT for cgi", *_clients[i]._serverPtr);
			_cgis[i].getClient()->error("504", "Gateway Timeout");
			_cgis[i].closeCGI(i);
			i--;
		}
	}
}

void Settings::closeClient(unsigned int i)
{
	for(unsigned int j = i + 1; j < _clients.size(); j++)
	{
			_clients[j].setId(j - 1);
			_clients[j].setClient(_clients.begin() + j);
	}
	close(_clients[i].getFd());
	_fds.erase(_fds.begin() + _servers.size() + i);
	Print::print(DEBUG, "Client " + to_string(i) + " closed. Socket " + to_string(_clients[i].getFd()) + " freed.", *_clients[i]._serverPtr);
	_clients.erase(_clients.begin() + i);
	for(std::vector<CGI>::iterator j = _cgis.begin(); j != _cgis.end(); j++)
	{
		if(j->getID() == (int)i)
			{
				_fds.erase(_fds.begin() + _servers.size() + _clients.size() + (j - _cgis.begin()));
				_cgis.erase(j);
				j--;
				continue;
			}
		else if(j->getID() > (int)i)
			j->setID(j->getID() - 1);
		j->setClient(_clients.begin() + j->getID());
	}
}

void Settings::addClient(Server &serv)
{
	try
	{
		Client test(&serv,  _clients.size(), this);
		_clients.push_back(test);
		for(std::vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
			i->setClient(i);
		for(std::vector<CGI>::iterator i = _cgis.begin(); i != _cgis.end(); i++)
			i->setClient(_clients.begin() + i->getID());
		_clients.back().setClient(_clients.end() - 1);
		pollfd tmp = {_clients.back().getFd(), POLLIN, 0};
		_fds.insert(_fds.begin() + _servers.size() - 1 + _clients.size(), tmp);
	}
	catch(const std::exception& e)
	{
		Print::print(ERROR, e.what());
	}
}

Server* Settings::find_server_name(std::string host)
{
	Server* server = NULL;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		for (size_t j = 0; j < _servers[i].getNameNumber(); i++)
		{
			if (host == _servers[i].getName(j))
			{
				server = &_servers[i];
				return server;
			}
		}
	}
	return server;
}
