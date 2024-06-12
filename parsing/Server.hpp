/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:35:46 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 16:09:49 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <arpa/inet.h>
#include "Route.hpp"
#include "../Utils.hpp"
#include "../Print.hpp"

class Server
{
    public:
        Server();
        std::string const &getName(void) const;
        std::string const &getHost(void) const;
        uint16_t getPort(void) const;
        uint64_t getMaxBodySize(void) const;
		int	getID(void) const;
        std::string const &getErrorPage(std::string const &error);

        void	parse(std::string const& content, std::string::iterator& start, std::string::iterator& end);
    private:
		int	id;
        std::vector<std::string> _name;
        in_addr_t _host;
        uint16_t _port;
        uint64_t _max_body_size;
        std::map<std::string, std::string> _error_pages;
        // std::vector<Route> _routes;

		void listen(std::string const& content, std::string::iterator& start);
		void servername(std::string const& content, std::string::iterator& start);
		void maxbodysize(std::string const& content, std::string::iterator& start);
};