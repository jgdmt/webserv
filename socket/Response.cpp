/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/06/28 16:33:49 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"
#include "CGI.hpp"

Response::Response(Request *req, Server *serv, CGI *cgi)
{
    _req = req;
    _serv = serv;
	_cgi = cgi;
}

Response::Response(Response const &res)
{
    this->_req = res._req;
    this->_serv = res._serv;
    *this = res;
}

Response &Response::operator=(Response const &res)
{
    this->_buffer = res._buffer;
    this->_cgiEnv = res._cgiEnv;
    return *this;
}

std::string *Response::getRes(void)
{
    return &_buffer;
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
    _buffer = "HTTP/1.1 " + type + "\r\n";
    if (_req->getConnection() == "keep-alive\r")
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
    if (_req->getMethod() == "HEAD")
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
    std::string path = _serv->getErrorPage(httpErrorCode);
    genBody(path);
    Print::print(DEBUG, "Error " + to_string(httpErrorCode) + " sent.", *_serv);
    return;
}

bool Response::checkCGI(std::string path, Route *route)
{
    if (!route->getCgiPath().empty() && route->getCgiLength() > 0 && (_req->getMethod() == "POST" || _req->getMethod() == "GET"))
    {
        std::string ext = path.substr(path.find_last_of('.'));

        for (size_t i = 0; i < route->getCgiLength(); i++)
        {
            if (ext == route->getCgiExtension(i))
            {
                genHeader("200 OK");
                _buffer.append(_cgi->handler(route, path));
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
            return;
        unsigned int i = 0;
        std::string myme = MIME_TYPE(path.substr(path.find_last_of('.') + 1));
        while (i < _req->getAcceptSize())
        {
            if (myme == _req->getAccept(i) || "*/*" == _req->getAccept(i))
                break;
            i++;
        }
        if (_req->getAcceptSize() != 0 && i == _req->getAcceptSize())
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

    stat(path.c_str(), &path_stat);
    if (!S_ISDIR(path_stat.st_mode))
       genRes(path, route);
    else
    {
        if (path[path.size() - 1] != '/')
        {
            genHeader("301 Moved Permanently");
            _buffer.append("Location: " + _req->getUri() + "/\r\n\r\n");
            return;
        }
        if (route->getdefaultFileForDirectory().size() != 0 && route->getPath() + "/" == path)
        {
            if (route->getdefaultFileForDirectory()[0] == '/')
                route->setdefaultFileForDirectory(route->getdefaultFileForDirectory().substr(1));
            genRes(path + route->getdefaultFileForDirectory(), route);
            return;
        }
        unsigned int bestMatchKey = _req->getAcceptSize();
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
                while (i < _req->getAcceptSize())
                {
                    if (myme == _req->getAccept(i) || "*/*" == _req->getAccept(i))
                        break;
                    i++;
                }
                if (i != _req->getAcceptSize() && i < bestMatchKey)
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
        else if ((route && route->IsListing()))
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
    size_t next = _req->getUri().find('/', pos + 1);
    if (next == std::string::npos)
        next = _req->getUri().size();
    std::string chunkUri = _req->getUri().substr(pos, next - pos);
    for (unsigned int i = 0; i < _serv->getRoutesNumber(); i++)
    {
        if (_serv->getRoute(i)->getRedirection() == chunkUri)
        {
            route = _serv->getRoute(i);
            break;
        }
    }
    while (route != NULL)
    {
        pos = next;
        next = _req->getUri().find('/', pos + 1);
        if (next == std::string::npos)
            next = _req->getUri().size();
        chunkUri = _req->getUri().substr(pos, next - pos);
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
            break;
    }
    std::string path;
    if (route != NULL)
    {
        if (route && !route->isAutorizedMethod(_req->getMethod()))
        {
            error("405", "Method Not Allowed");
            return;
        }
        if (route->getPath()[route->getPath().size() - 1] == '/')
            route->setPath(route->getPath().substr(0, route->getPath().size() - 1));
        path = route->getPath();
        if (pos < _req->getUri().size())
            path.append(_req->getUri().substr(pos, _req->getUri().size() - pos));
        check_path(path, route);
    }
}
