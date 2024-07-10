/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:35:46 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 20:15:40 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include "Route.hpp"
#include "../Utils.hpp"
#include "../Print.hpp"

class Server
{
    public:
        Server(int id);
		~Server();
        Server(Server const &cpy);
        Server &operator=(Server const &cpy);
        std::string const &getName(void) const;
        in_addr_t const &getHost(void) const;
        uint16_t getPort(void) const;
        uint64_t getMaxBodySize(void) const;
		int	getID(void) const;
        std::string const &getErrorPage(std::string const &error);
		size_t getNameNumber(void) const;
        std::string getLogLevel(void) const;
		std::string const &getName(int i) const;
        int getFdListen(void) const;
        Route* getRoute(unsigned int i);
        unsigned int getRoutesNumber(void) const;

        void	parse(std::string const& content, std::string::iterator& start, std::string::iterator& end);
        void    setup(void);
    private:
		int	_id;
		std::vector<std::string> _name;
        in_addr_t _host;
        uint16_t _port;
        uint64_t _max_body_size;
        std::vector<Route> _routes;
        std::map<std::string, std::string> _error_pages;
        std::string _log_level;
        struct sockaddr_in _server_socket;
        int _fd_listen;

		void listen2(std::string const& content, std::string::iterator& start);
		void servername(std::string const& content, std::string::iterator& start);
		void maxbodysize(std::string const& content, std::string::iterator& start);
		void errorpage(std::string const& content, std::string::iterator& start);
		void parseRoot(std::string const& content, std::string::iterator& start, std::string::iterator& end);
        void loglevel(std::string const& content, std::string::iterator& start);
};