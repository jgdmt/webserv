/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/07/02 17:53:18 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"
#include "CGI.hpp"
#include "../parsing/Settings.hpp"
#include "Client.hpp"



Response::Response()
{
	_cgiStatus = false;
}
void Response::setClient(std::vector<Client>::iterator cli)
{
    _client = cli;
}

Response::~Response()
{}

Response::Response(Response const &res)
{
    *this = res;
}

void Response::addBuffer(std::string& body)
{
	_buffer.append(body);
	std::cout << _buffer << "\n";
}

void Response::cut(int pos)
{
    _buffer = _buffer.substr(pos, _buffer.size() - pos);
    _buffer.shrink_to_fit();
}

void Response::genHeader(std::string type)
{
    std::time_t now = std::time(NULL);
    std::tm *time = std::localtime(&now);
    char buff[80];

    _buffer.clear();
	_cgiStatus = false;
    _buffer = "HTTP/1.1 " + type + "\r\n";
    if (_client->getConnection() == "keep-alive\r")
    {
        _buffer.append("Connection: Keep-Alive\r\n");
        _buffer.append("Keep-Alive: timeout=60\r\n");
    }
    strftime(buff, 80, "%c", time);
    _buffer.append("Date: " + (std::string)buff + "\r\n");
    _buffer.append("Server: IsWatchingYou\r\n");
}

void Response::genBody(std::string path)
{
    std::ifstream FileDescriptor(path.c_str());
    if (FileDescriptor.fail())
    {
        Print::print(ERROR, "Couldn't open body file");
        error("500", "Internal Server Error");
        return;
    }
    std::ostringstream FileStream;
    FileStream << FileDescriptor.rdbuf();
    std::string buff = FileStream.str();
    _buffer.append("Content-Type: " + (std::string)MIME_TYPE(path.substr(path.find_last_of('.') + 1)) + "\r\n");
    _buffer.append("Content-Length: " + to_string(buff.length()) + "\r\n\r\n");
    if (_client->getMethod() == "HEAD")
        return;
    _buffer.append(buff);
}

void Response::genDirListing(std::string path)
{
    _buffer.append("Content-Type: text/html\r\n");
    std::string body;
    body.append("<html><head><title>Index of " + path + "</title></head>");
    body.append("<body><h1>Index of " + path + "</h1><hr><pre>");
    DIR *dir = opendir(path.c_str());
    struct stat path_stat;
    std::string relativeUrl;
    if (!dir)
    {
        Print::print(ERROR, "Couldn't open directory");
        error("500", "Internal Server Error");
        return;
    }
    struct dirent *file;
    while ((file = readdir(dir)) != NULL)
    {
        if ((std::string)file->d_name == ".")
            continue;
        stat((path + file->d_name).c_str(), &path_stat);
        relativeUrl = file->d_name;
        if (S_ISDIR(path_stat.st_mode))
            relativeUrl.append("/");
        body.append("<a href=\"" + relativeUrl + "\">" + relativeUrl + "</a>\n");
    }
    body.append("</pre><hr></body></html>");
    _buffer.append("Content-Length: " + to_string(body.size()) + "\r\n");
    _buffer.append("\r\n");
    _buffer.append(body);
}

void Response::error(std::string httpErrorCode, std::string httpErrorMessage)
{
    genHeader(httpErrorCode + ' ' + httpErrorMessage);
    std::string path = _client->_serverPtr->getErrorPage(httpErrorCode);
    genBody(path);
    Print::print(DEBUG, "Error " + to_string(httpErrorCode) + " sent.", *_client->_serverPtr);
    return;
}

bool Response::checkCGI(std::string path, Route *route)
{
    if (!route->getCgiPath().empty() && route->getCgiLength() > 0 && (_client->getMethod() == "POST" || _client->getMethod() == "GET"))
    {
        std::string ext = path.substr(path.find_last_of('.'));

        for (size_t i = 0; i < route->getCgiLength(); i++)
        {
            if (ext == route->getCgiExtension(i))
            {
				_client->_settingsPtr->getCgi()->push_back(CGI(_client));
                genHeader("200 OK");
                _client->_settingsPtr->getCgi()->back().handler(route, path);
                return true;
            }
        }
        return false;
    }
    else
        return false;
}
void Response::genRes(std::string path, Route* route)
{
    if (access(path.c_str(), F_OK))
        error("404", "Not Found");
    else if (access(path.c_str(), R_OK))
        error("403", "Forbidden");
    else
    {
        if (checkCGI(path, route))
        {
			_cgiStatus = true;
			return ;
		}
        unsigned int i = 0;
        std::string myme = MIME_TYPE(path.substr(path.find_last_of('.') + 1));
        while (i < _client->getAcceptSize())
        {
            if (myme == _client->getAccept(i) || "*/*" == _client->getAccept(i))
                break;
            i++;
        }
        if (_client->getAcceptSize() != 0 && i == _client->getAcceptSize())
            error("406", "Not Acceptable");
        else
        {
            genHeader("200 OK");
            genBody(path);
        }
    }
}

void Response::check_path(std::string path, Route *route)
{
    struct stat path_stat;

    std::cout << "Path: " << path << std::endl;
    if(stat(path.c_str(), &path_stat) == -1)
    {
        error("500", "Internal Server Error");
        return ;
    }
    if (!S_ISDIR(path_stat.st_mode))
       genRes(path, route);
    else
    {

        if (path[path.size() - 1] != '/' && _client->getUri().size() != 1)
        {
            genHeader("301 Moved Permanently");
            _buffer.append("Location: " + _client->getUri() + "/\r\n\r\n");
            return;
        }
        if (route->getdefaultFileForDirectory().size() != 0 && route->getPath() + "/" == path)
        {
            if (route->getdefaultFileForDirectory()[0] == '/')
                route->setdefaultFileForDirectory(route->getdefaultFileForDirectory().substr(1));
            genRes(path + route->getdefaultFileForDirectory(), route);
            return;
        }
        unsigned int bestMatchKey = _client->getAcceptSize();
        std::string bestMatchValue;
        DIR *dir = opendir(path.c_str());
        if (!dir)
        {
            Print::print(ERROR, "Couldn't open directory");
            error("500", "Internal Server Error");
            return;
        }
        struct dirent *file;
        while ((file = readdir(dir)) != NULL)
        {
            std::string fileName = file->d_name;
            if (fileName.find("index.") == 0)
            {
                stat((path + file->d_name).c_str(), &path_stat);
                if (S_ISDIR(path_stat.st_mode))
                    continue;
                std::string tmp = file->d_name;
                std::string myme = MIME_TYPE(tmp.substr(tmp.find_last_of('.') + 1));
                unsigned int i = 0;
                while (i < _client->getAcceptSize())
                {
                    if (myme == _client->getAccept(i) || "*/*" == _client->getAccept(i))
                        break;
                    i++;
                }
                if (i != _client->getAcceptSize() && i < bestMatchKey)
                {
                    bestMatchKey = i;
                    bestMatchValue = tmp;
                }
            }
        }
        if (bestMatchValue.size())
        {
            if (path[path.size() - 1] != '/')
                path.append("/");
            genRes(path + bestMatchValue, route);
        }
        else if ( route->IsListing())
        {
            genHeader("200 OK");
            genDirListing(path);
        }
        else
        {
            error("403", "Forbidden");
        }
        
    }
}

void Response::init(void)
{
    size_t pos = 0;
    Route *route = NULL;
    size_t next = _client->getUri().find('/', pos + 1);
    if (next == std::string::npos)
        next = _client->getUri().size();
    std::string chunkUri = _client->getUri().substr(pos, next - pos);
    for (unsigned int i = 0; i < _client->_serverPtr->getRoutesNumber(); i++)
    {
        if (_client->_serverPtr->getRoute(i)->getRedirection() == chunkUri)
        {
            route = _client->_serverPtr->getRoute(i);
            break;
        }
    }
    if (route == NULL)
    {
        for (unsigned int i = 0; i < _client->_serverPtr->getRoutesNumber(); i++)
        {
            if (_client->_serverPtr->getRoute(i)->getRedirection() == "/")
            {
                route = _client->_serverPtr->getRoute(i);
				next = route->getRedirection().length();
                break;
            }
        }
    }
    while (route != NULL)
    {
        // int oldPos = pos;
        pos = next;
        next = _client->getUri().find('/', pos + 1);
        if (next == std::string::npos)
            next = _client->getUri().size();
        chunkUri = _client->getUri().substr(pos, next - pos);
        unsigned int nbRoute = route->getRoutesNumber();
        unsigned int i = 0;
        while (i < nbRoute)
        {
            if (route->getRoute(i)->getRedirection() == chunkUri)
            {
                route = route->getRoute(i);
                break;
            }
            i++;
        }
        if (i == nbRoute)
        {
            // pos = oldPos;
            break;
        }
    }
    std::string path;
    if (route != NULL)
    {
        if (!route->isAutorizedMethod(_client->getMethod()))
        {
            error("405", "Method Not Allowed");
            return;
        }
        std::cout << "Route: " << route->getRedirection() << std::endl;
		std::cout << "Route_path: " << route->getPath() << "\n";
        if (route->getPath()[route->getPath().size() - 1] == '/' && _client->getUri()[0] == '/')
            route->setPath(route->getPath().substr(0, route->getPath().size() - 1));
        path = route->getPath();
        if (pos < _client->getUri().size())
            path.append(_client->getUri().substr(pos, _client->getUri().size() - pos));
        check_path(path, route);
    }
    else
    {
       error("404", "Not Found");
    }
}
