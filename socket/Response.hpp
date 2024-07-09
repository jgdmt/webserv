/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/09 18:55:02 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <ctime>
#include <fstream>
#include <map>
#include <cstdio>
#include <dirent.h>
#include "../Print.hpp"
#include "Request.hpp"
#include "../parsing/Server.hpp"
#include "CGI.hpp"

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

class Settings;
class Client;

class Response
{
    public:
        Response();
        Response(Response const &res);
        // Response &operator=(Response const &res);
        ~Response();
        void init(void);
        void error(std::string httpErrorCode, std::string httpErrorMessage);
        void cut(int pos);
		void addBuffer(std::string& body);
        void setClient(std::vector<Client>::iterator cli);
    protected:
        std::vector<Client>::iterator getClient(void) const;
        std::string _buffer;
		bool _cgiStatus;
    private:
        std::vector<Client>::iterator _client;
        bool checkCGI(std::string path, Route *route);
        void genHeader(std::string type);
        void genBody(std::string path);
        void genDirListing(std::string path);
        void genRes(std::string path, Route* route);
        void deleteHandle(std::string path);
        void check_path(std::string path, Route *route);
};