/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:00:05 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 16:16:09 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string const &name): _name(name)
{
    _host = "*";
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

std::string const &Server::getName(void) const
{
    return _name;
}

std::string const &Server::getHost(void) const
{
    return _host;
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