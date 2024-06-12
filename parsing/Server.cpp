/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:00:05 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/12 17:39:15 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
Server::Server()
{
    _port = 80;
    _max_body_size = 1000000000;
    _error_pages["400"] = "default/http_error/400.html";
    _error_pages["403"] = "default/http_error/403.html";
    _error_pages["404"] = "default/http_error/404.html";
    _error_pages["405"] = "default/http_error/405.html";
    _error_pages["410"] = "default/http_error/410.html";
    _error_pages["413"] = "default/http_error/413.html";
    _error_pages["500"] = "default/http_error/500.html";

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
    return id;
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

void Server::listen(std::string const& content, std::string::iterator& start)
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

	std::cout << "LENGTH "<<len<< "\n "; 
	if (len == -1)
	{
		Print::error_print(ERROR, "Error: missing ';'");
		exit (0);
	}
	while (start != tmp + len)
	{
		it = std::find(start, start + len, ' ');
		std::cout << content.substr(start - content.begin(), it - start) << ".\n";
		this->_name.push_back(content.substr(start - content.begin(), it - start));
		start = it;
		while (*start == ' ')
			start++;
	}
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
		while (*start != ' ' && *start != ';')
			start++;
		param = content.substr(it - content.begin(), start - it);
		// std::cout << "'" << param << "'\n";
		while (*start == ' ')
			start++;
		if (param == "listen")
			listen(content, start);
		else if (param == "server_name")
			servername(content, start);
		else
			start++;
			// exit(0);
		while (*start == ' ')
			start++;
	}
	
}