/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:31:30 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/28 16:09:46 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include "parsing/Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#define READSIZE 50000

class Settings;

class Client
{
    public:
        Client(Server &serv, int id, Settings* settings);
        ~Client(void);
        int getFd(void) const;
        time_t getLastCom(void) const;
        Server &getServer(void);
        void    setId(int id);
        void    readRequest(void);
        void    sendResponse(void);
        Client(Client const &client);
        Client &operator=(Client const &client);
    private:
		Settings *_settings;
        Request req;
        Server _serv;
		CGI _cgi;
        Response res;
        int _id;
        int _fd;
        sockaddr_in _addr;
        time_t _last_com;
};