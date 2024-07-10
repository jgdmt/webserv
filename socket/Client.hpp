/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:31:30 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 17:00:01 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include "../parsing/Server.hpp"
#include "../Print.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#define READSIZE 50000
#define WRITESIZE 50000

class Settings;

class Client: public Request, public Response
{
    public:
        Client(Server *serv, int id, Settings* settings);
        ~Client(void);
        int getFd(void) const;
		int getId(void) const;
        time_t getLastCom(void) const;
        void    setId(int id);
        void    readRequest(void);
        void    sendResponse(void);
        sockaddr_in* getAddr();
        Client(Client const &client);
        Client &operator=(Client const &client);
		Settings *_settingsPtr;
        Server *_serverPtr;
    private:
        int _id;
        int _fd;
        sockaddr_in _addr;
        time_t _last_com;
};