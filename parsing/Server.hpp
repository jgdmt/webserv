/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:35:46 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 13:00:35 by vilibert         ###   ########.fr       */
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
#include "Route.hpp"
#include "../Utils.hpp"
#include "../Print.hpp"

class Server
{
    public:
        Server(int id);
		~Server();
        std::string const &getName(void) const;
        in_addr_t const &getHost(void) const;
        uint16_t getPort(void) const;
        uint64_t getMaxBodySize(void) const;
		int	getID(void) const;
        std::string const &getErrorPage(std::string const &error);
        std::string getLogLevel(void) const;

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

		void parseRoot(std::string const& content, std::string::iterator& start, std::string::iterator& end);
};