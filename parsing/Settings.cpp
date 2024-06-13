/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:00:33 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 19:33:53 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include "../Print.hpp"
#include <iterator>

typedef std::string::iterator it;

static std::string createContent(std::string const& fileName)
{
	std::ifstream ifs(fileName);
	std::string	line;
	std::string content;

	if (!ifs || !ifs.is_open())
	{
		Print::error_print(ERROR, "File error.");
		ifs.open("default/webserv.conf");
	}
	while (ifs.good())
	{
		std::getline(ifs, line);
		content += line;
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
		Print::error_print(CRASH, "Bracket error: } not found");
	return (i);
}

void Settings::parseServer(std::string const& content, it& name, it& start, it& end)
{
	int len = Route::find_len(content, name, '{', true);
	
	if (name == start || content.substr(name - content.begin(), len) != "server")
		Print::error_print(CRASH, content.substr(name - content.begin(), len) + " is an unknown key");
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
		Print::error_print(CRASH, "Parsing: file is empty");
	while (start != content.end())
	{
		name = start;
		while (start != content.end() && *start != '{')
		{
			if (*start == '}')
				Print::error_print(CRASH, "Bracket error: } has no {");
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
}

std::vector<Server> &Settings::getServers(void)
{
	return _servers;
}

void Settings::setup(void)
{
	char buffer[INET_ADDRSTRLEN];
	for(int i = 0; i < (int)_servers.size(); i++)
	{
		_servers[i].setup();
		pollfd tmp = {_servers[i].getFdListen(), POLLIN, 0};
		_fds.push_back(tmp);
		Print::print(INFO, _servers[i], "Server started! Listen at " + (std::string)inet_ntop(AF_INET, &_servers[i].getHost(), buffer, INET_ADDRSTRLEN) + ":" + to_string<uint16_t>(_servers[i].getPort()) + ".");
	}
	Print::print(SUCCESS,"All servers succesfully started !");
}

void Settings::run(void)
{
	while(1)
	{
		if(poll(_fds.data(), _fds.size(), -1) == -1)
			Print::error_print(CRASH, "Poll: " + (std::string)strerror(errno));
		for(unsigned int i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents == POLLERR || _fds[i].revents == POLLHUP || _fds[i].revents == POLLNVAL)
				Print::error_print(CRASH, "Poll: " + (std::string)strerror(errno));
			else if(_fds[i].revents == POLLIN && _servers.size() > i && _servers[i].getFdListen() == _fds[i].fd)
				addClient(i, _servers[i]);
			else if (_fds[i].revents == POLLIN && _clients.size() > (i - _servers.size()) && _clients[i].getFd() == _fds[i].fd)
				_clients[i].readRequest();
		}
		checkTimeout();
	}
}

void Settings::checkTimeout(void)
{
	Print::print(INFO, "A checker");
}

void Settings::addClient(unsigned int i, Server &serv)
{
	Client test(serv);
	
	pollfd tmp = {test.getFd(), POLLIN, 0};
	_fds.push_back(tmp);
	_clients.push_back(test);

}