/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:31:30 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/14 11:21:24 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include "parsing/Server.hpp"

class Client
{
    public:
        Client(Server &serv);
        ~Client(void);
        int getFd(void) const;
        time_t getLastCom(void) const;
        Server &getServer(void);
        void    readRequest(void);
        Client(Client const &client);
        Client &operator=(Client const &client);
    private:
        int _fd;
        sockaddr_in _addr;
        Server _serv;
        time_t _last_com;
};