/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:00:05 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 18:55:36 by jgoudema         ###   ########.fr       */
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

void Server::listen2(std::string const& content, std::string::iterator& start)
{
	std::string::iterator found;
	std::string	host;
	std::string port;
	int l = Route::find_len(content, start, ';', false);
	sockaddr_in serverAddress;

	if (l == 0)
		Print::error_print(CRASH, "Parsing server: listen is missing a value");
	if (l == -1)
		Print::error_print(CRASH, "Parsing server: missing ';'");
	else if (l == -2)
		Print::error_print(CRASH, "Parsing server: listen does not take multiple values");
	found = std::find(start, start + l, ':');
	if (found == start + l)
		Print::error_print(CRASH, "Parsing server: listen is missing port parameter");
	host = content.substr(start - content.begin(), found - start);
	if (host == "localhost")
		host = "127.0.0.1";
	if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr.s_addr) == -1)
		Print::error_print(CRASH, "Parsing server: wrong host");
	port = content.substr(found - content.begin() + 1, std::find(start, start + l, ';') - 1 - found);
	for (size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
			Print::error_print(CRASH, "Parsing server: wrong port");
		if (convertType<int>(port) > 65535)
			Print::error_print(CRASH, "Parsing server: port overflow");
	}
	start += l;
	this->_host = serverAddress.sin_addr.s_addr;
	this->_port = convertType<uint16_t>(port);
}

void	Server::servername(std::string const& content, std::string::iterator& start)
{
	int	len = Route::find_len(content, start, ';', true);
	std::string::iterator tmp = start;
	std::string::iterator it;

	if (len == 0)
		Print::error_print(CRASH, "Parsing server: server_name is missing a value");
	if (len == -1)
		Print::error_print(CRASH, "Parsing server: missing ';'");
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
	int len = Route::find_len(content, start, ';', false);

	if (len == 0)
		Print::error_print(CRASH, "Parsing server: max_body_size is missing a value");
	if (len == -1)
		Print::error_print(CRASH, "Parsing server: missing ';'");
	if (len == -2)
		Print::error_print(CRASH, "Parsing server: max_body_size does not take multiple values");
	this->_max_body_size = convertType<uint64_t>(content.substr(start - content.begin(), (size_t)len));
	start += len;
}

void	Server::parseRoot(std::string const& content, std::string::iterator& start, std::string::iterator& end)
{
	int l = Route::find_len(content, start, '{', false);

	if (l == 0)
		Print::error_print(CRASH, "Parsing server: location is missing a value");
	if (l == -2)
		Print::error_print(CRASH, "Parsing server: location does not take multiple parameters");
	Route route;
	route.parse(content, start, end, l);
	this->_routes.push_back(route);
}

void	Server::errorpage(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', true);
	std::string::iterator tmp = start;
	std::string::iterator it;
	std::string key;
	std::string link;

	if (len == 0)
		Print::error_print(CRASH, "Parsing server: error_page is missing a value");
	if (len == -1)
		Print::error_print(CRASH, "Parsing server: missing ';'");
	it = std::find(start, tmp + len, ' ');
	if (it == tmp + len)
		Print::error_print(CRASH, "Parsing server: error_page is missing a value");
	key = content.substr(start - content.begin(), it - start);
	start = it;
	while (*start == ' ')
		start++;
	it = std::find(start, tmp + len, ' ');
	link = content.substr(start - content.begin(), it - start);
	start = it;
	while (*start == ' ')
		start++;
	if (*start != ';')
		Print::error_print(CRASH, "Parsing server: error_page takes two values only");
	if (this->_error_pages.find(key) == this->_error_pages.end())
		Print::error_print(CRASH, "Parsing server: " + key + " is an unknown error");
	if (access(link.c_str(), F_OK | R_OK))
		Print::error_print(CRASH, "Parsing server: " + link);
	this->_error_pages[key] = link;
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
		else if (param == "error_page")
			errorpage(content, start);
		else
			Print::error_print(CRASH, "Parsing server: " + param + " is unknown");
		while (*start == ' ' || *start == ';')
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
}