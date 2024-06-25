/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/25 10:57:41 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <ctime>
#include <fstream>
#include <map>
#include <dirent.h>
#include "../Print.hpp"
#include "Request.hpp"
#include "../parsing/Server.hpp"

#define WRITESIZE 50000

#define MIME_TYPE(extension) \
    (std::string(extension) == "html" ? "text/html" : \
    std::string(extension) == "htm" ? "text/html" : \
    std::string(extension) == "txt" ? "text/plain" : \
    std::string(extension) == "css" ? "text/css" : \
    std::string(extension) == "js" ? "application/javascript" : \
    std::string(extension) == "json" ? "application/json" : \
    std::string(extension) == "png" ? "image/png" : \
    std::string(extension) == "jpg" ? "image/jpeg" : \
    std::string(extension) == "jpeg" ? "image/jpeg" : \
    std::string(extension) == "gif" ? "image/gif" : \
    std::string(extension) == "pdf" ? "application/pdf" : \
    std::string(extension) == "zip" ? "application/zip" : \
    std::string(extension) == "xml" ? "application/xml" : \
    std::string(extension) == "mp3" ? "audio/mpeg" : \
    std::string(extension) == "mp4" ? "video/mp4" : \
    std::string(extension) == "webp" ? "image/webp" : \
    std::string(extension) == "webm" ? "video/webm" : \
    std::string(extension) == "php" ? "application/x-httpd-php" : \
    std::string(extension) == "svg" ? "image/svg+xml" : \
    std::string(extension) == "ico" ? "image/vnd.microsoft.icon" : \
    "application/octet-stream" )

class Response
{
    public:
        Response(Request* req, Server* serv);
        Response(Response const &res);
        Response &operator=(Response const &res);
        void init(void);
        void error(std::string httpErrorCode, std::string httpErrorMessage);
        void cut(int pos);
        std::string *getRes(void);
    private:
        Request* _req;
        Server* _serv;
        std::string _buffer;
		std::map<std::string, std::string> _cgiEnv;
        
        void genHeader(std::string type);
        void genBody(std::string path);

        void createEnv(void);
        void check_path(std::string path);
};