/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:31:30 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/21 11:14:23 by vilibert         ###   ########.fr       */
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

#define READSIZE 50000

class Settings;

class Client
{
    public:
        Client(Server &serv, int id);
        ~Client(void);
        int getFd(void) const;
        time_t getLastCom(void) const;
        Server &getServer(void);
        void    readRequest(Settings *set);
        void    sendResponse(Settings *set);
        Client(Client const &client);
        Client &operator=(Client const &client);
    private:
        Request req;
        Server _serv;
        Response res;
        const int _id;
        int _fd;
        sockaddr_in _addr;
        time_t _last_com;
};