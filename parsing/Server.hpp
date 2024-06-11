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
#include "Route.hpp"

class Server
{
    public:
        Server(std::string const &name);
        std::string const &getName(void) const;
        std::string const &getHost(void) const;
        uint16_t getPort(void) const;
        uint64_t getMaxBodySize(void) const;
        std::string const &getErrorPage(std::string const &error);

        
    private:
        const std::string _name;
        std::string _host;
        uint16_t _port;
        uint64_t _max_body_size;
        std::map<std::string, std::string> _error_pages;
        // std::vector<Route> _routes;
        Server();

};