/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/19 16:14:57 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <ctime>
#include <fstream>
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
        
        void genHeader(std::string type);
        void genBody(std::string path);

        void rec();
};