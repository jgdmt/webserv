/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:51:08 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/28 16:36:51 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "Server.hpp"
#include <poll.h>
#include "../socket/Client.hpp"

#define TIMEOUT 60

class Settings
{
    public:
        void parse(std::string const &filePath);
        std::vector<Server> *getServers(void);
        std::vector<pollfd> *getFds();
		std::vector<CGI> *getCgi();
        void setup(void);
        void run(void);
        void closeClient(unsigned int i);
    private:
        void checkTimeout(void);
		void parseServer(std::string const& content, std::string::iterator& name, std::string::iterator &start, std::string::iterator &end);
        void addClient(unsigned int i, Server &serv);
        std::vector<pollfd> _fds;
        std::vector<Server> _servers;
        std::vector<Client> _clients;
		std::vector<CGI> _cgis;
}; 