/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:00:05 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 13:01:26 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
Server::Server(int id) : _id(id)
{
    _port = 80;
	_log_level= DEBUG;
    _max_body_size = 1000000000;
    _error_pages["400"] = "default/http_error/400.html";
    _error_pages["403"] = "default/http_error/403.html";
    _error_pages["404"] = "default/http_error/404.html";
    _error_pages["405"] = "default/http_error/405.html";
    _error_pages["410"] = "default/http_error/410.html";
    _error_pages["413"] = "default/http_error/413.html";
    _error_pages["500"] = "default/http_error/500.html";

}

Server::~Server()
{
	close(this->_fd_listen);
}

uint16_t Server::getPort(void) const
{
    return _port;    
}

uint64_t Server::getMaxBodySize(void) const
{
    return _max_body_size;
}

in_addr_t const &Server::getHost(void) const
{
	return _host;
}

std::string const &Server::getErrorPage(std::string const &error)
{
    return _error_pages[error];
}

int Server::getID(void) const
{
    return _id;
}

std::string Server::getLogLevel(void) const
{
	return _log_level;
}

static int	find_len(std::string const& content, std::string::iterator const& name, char endc, bool split)
{
	int	len = 0;

	while ((name + len) != content.end() && *(name + len) != endc)
	{
		if (*(name + len) == ' ')
		{
			int	space = 0;
			while (*(name + len + space) == ' ' && (name + len + space) != content.end())
				space++;
			if (*(name + len + space) != endc)
				len += space;
			else
				return (len);
			if (name + len == content.end())
				return (-1);
			if (!split && *(name + len) != endc && *(name + len) != ' ')
				return (-2);
		}
		len++;
	}
	if ((name + len) == content.end())
		return (-1);
	return (len);
}

void Server::listen2(std::string const& content, std::string::iterator& start)
{
	std::string::iterator found;
	std::string	host;
	std::string port;
	int l = find_len(content, start, ';', false);
	sockaddr_in serverAddress;

	if (l == -1)
	{
		Print::error_print(ERROR, "Error: missing ';'");
		exit (0);
	}
	else if (l == -2)
	{
		Print::error_print(ERROR, "Error: listen does not take more than one parameter");
		exit (0);
	}
	found = std::find(start, start + l, ':');
	if (found == start + l)
	{
		Print::error_print(ERROR, "Error: listen is missing port parameter");
		exit (0);
	}
	host = content.substr(start - content.begin(), found - start);
	if (host == "localhost")
		host = "127.0.0.1";
	if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr.s_addr) == -1)
	{
		Print::error_print(ERROR, "Error: host is wrong");
		exit(0);
	}
	port = content.substr(found - content.begin() + 1, std::find(start, start + l, ';') - 1 - found);
	for (size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
		{
			Print::error_print(ERROR, "Error: port is wrong");
			exit (0);
		}
		if (convertType<int>(port) > 65535)
		{
			Print::error_print(ERROR, "Error: port overflow");
			exit(0);
		}
	}
	start += l;
	this->_host = serverAddress.sin_addr.s_addr;
	this->_port = convertType<uint16_t>(port);
}

void	Server::servername(std::string const& content, std::string::iterator& start)
{
	int	len = find_len(content, start, ';', true);
	std::string::iterator tmp = start;
	std::string::iterator it;

	if (len == -1)
	{
		Print::error_print(ERROR, "Error: missing ';'");
		exit (0);
	}
	while (start != tmp + len)
	{
		it = std::find(start, tmp + len, ' ');
		this->_name.push_back(content.substr(start - content.begin(), it - start));
		start = it;
		while (*start == ' ')
			start++;
	}
}

void Server::maxbodysize(std::string const& content, std::string::iterator& start)
{
	int len = find_len(content, start, ';', false);

	if (len == -1)
	{
		Print::error_print(ERROR, "Error: missing ';'");
		exit (0);
	}
	if (len == -2)
	{
		Print::error_print(ERROR, "Error: max_body_size doesn't take multiple parameters");
		exit (0);
	}
	this->_max_body_size = convertType<uint64_t>(content.substr(start - content.begin(), (size_t)len));
}

void	Server::parseRoot(std::string const& content, std::string::iterator& start, std::string::iterator& end)
{
	int l = find_len(content, start, '{', false);

	if (l == -2)
	{
		Print::error_print(ERROR, "Error: max_body_size doesn't take multiple parameters");
		exit (0);
	}
	Route route;
	route.parse(content, start, end);
	this->_routes.push_back(route);
}

void	Server::parse(std::string const& content, std::string::iterator& start, std::string::iterator& end)
{
	std::string	param;
	std::string::iterator it;

	while (*(++start) == ' ')
		continue ;
	while (start != end)
	{
		it = start;
		while (*start != ' ' && *start != ';' && start != end)
			start++;
		if (start == end)
			return ;
		param = content.substr(it - content.begin(), start - it);
		std::cout << "'" << param << "'\n";
		while (*start == ' ')
			start++;
		if (param == "listen")
			listen2(content, start);
		else if (param == "server_name")
			servername(content, start);
		else if (param == "max_body_size")
			maxbodysize(content, start);
		else if (param == "location")
			parseRoot(content, start, end);
		else
			start++;
		while (*start == ' ')
			start++;
	}
	
}

void    Server::setup(void)
{
	_fd_listen = socket(AF_INET, SOCK_STREAM, 0);
    if(_fd_listen == -1)
		Print::error_print(CRASH, "Socket: " + (std::string)strerror(errno));
	_server_socket.sin_family = AF_INET;
    _server_socket.sin_port = htons(_port);
	_server_socket.sin_addr.s_addr = _host;
	int option = 1;
	if (setsockopt(_fd_listen, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1)
		Print::error_print(CRASH, "Socket: " + (std::string)strerror(errno));
    if(bind(_fd_listen, (struct sockaddr*)&_server_socket, sizeof(_server_socket)) == -1)
		Print::error_print(CRASH, "Bind: " + (std::string)strerror(errno));
	if (listen(_fd_listen, 512) == -1)	
		Print::error_print(CRASH, "Listen: " + (std::string)strerror(errno));
	if (fcntl(_fd_listen, F_SETFL, O_NONBLOCK) < 0)
		Print::error_print(CRASH, "Fcntl: " + (std::string)strerror(errno));
}