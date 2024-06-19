/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/19 18:45:20 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <ctime>
#include <fstream>
#include <map>
// #include <Windows.h>
#include "../Print.hpp"
#include "Request.hpp"
#include "../parsing/Server.hpp"

class Response
{
    public:
        Response(Request* req, Server* serv);
        void init(void);
        void error(std::string httpErrorCode, std::string httpErrorMessage);
    private:
        Request* _req;
        Server* _serv;
        std::string _buffer;
		std::map<std::string, std::string> _cgiEnv;
        
        void genHeader(std::string type);
        void genBody(std::string path);

        void createEnv(void);
};