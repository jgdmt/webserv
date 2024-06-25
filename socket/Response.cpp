/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/25 18:28:03 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CGI.hpp"

Response::Response(Request* req, Server* serv)
{
    _req = req;
    _serv = serv;
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
    std::tm *time =  std::localtime(&now);
    char buff[80];

    _buffer = "HTTP/1.1 " + type + "\r\n";
    if (_req->getConnection() == "keep-alive\r")
        _buffer.append("Connection: Keep-Alive\r\n");
    strftime(buff, 80, "%c", time);
    _buffer.append("Date: " + (std::string)buff + "\r\n");
    _buffer.append("Keep-Alive: timeout=60\r\n");
    _buffer.append("Server: IsWatchingYou\r\n");
}


void Response::genBody(std::string path)
{
    std::ifstream FileDescriptor(path.c_str());
    if(FileDescriptor.fail())
    {
        Print::print(ERROR, "Couldn't open body file");
        return ;
    }
    std::stringstream FileStream;
    FileStream << FileDescriptor.rdbuf();
    std::string buff = FileStream.str();
    _buffer.append("Content-Type: " + (std::string)MIME_TYPE(path.substr(path.find_last_of('.') + 1)) + "\r\n");
    _buffer.append("Content-Length: " + to_string(buff.size()) + "\r\n\r\n");

    _buffer.append(buff);
    _buffer.append("\r\n");
    // std::ofstream test(path + ".req");
    // test <<_buffer;

}

void Response::error(std::string httpErrorCode, std::string httpErrorMessage)
{
    genHeader(httpErrorCode + ' ' + httpErrorMessage);
    std::string path = _serv->getErrorPage(httpErrorCode);
    genBody(path);
    std::cout << "ERROOR "<< httpErrorCode << "\n";
    return;
}

void Response::check_path(std::string path, Route *route)
{
    struct stat	path_stat;
(void)route;
	stat(path.c_str(), &path_stat);
	if (!S_ISDIR(path_stat.st_mode))
    {
        if (access(path.c_str(), F_OK))
            error("404", "Not Found");
        else if (access(path.c_str(),  R_OK))
            error("403", "Forbidden");
        else
        {
            unsigned int i = 0;
            std::string myme = MIME_TYPE(path.substr(path.find_last_of('.') + 1));
			if (!route->getCgiPath().empty() && route->getCgiLength() > 0)
			{
				std::string ext = path.substr(path.find_last_of('.'));

				for (size_t i = 0; i < route->getCgiLength(); i++)
				{
					if (ext == route->getCgiExtension(i))
					{
						CGI cgi(*_req, *_serv, path);
						cgi.handler();
						break ;
					}
				}
			}
            while(i < _req->getAcceptSize())
            {
                if(myme == _req->getAccept(i) || "*/*" == _req->getAccept(i))
                    break;
                i++;
            }
            if(_req->getAcceptSize() != 0 && i == _req->getAcceptSize())
                error("406", "Not Acceptable");
            else
            {
                genHeader("200 OK");
                genBody(path);
            }
        }
    }
            // DIR *dir = opendirr()
            // for(int i = 0; i < )
}

void Response::init(void)
{
    size_t pos = 0;
    size_t next = _req->getUri().find('/', pos + 1);
    if (next == std::string::npos)
        next = _req->getUri().size();
    std::string chunkUri = _req->getUri().substr(pos, next - pos);
    Route *route = NULL;

    for(unsigned int i = 0; i < _serv->getRoutesNumber(); i++)
    {
        if(_serv->getRoute(i)->getRedirection() == chunkUri)
        {
            route = _serv->getRoute(i);
            break ;
        }
    }
    while (route != NULL)
    {
        pos = next;
        next = _req->getUri().find('/', pos + 1);
        if (next == std::string::npos)
            next = _req->getUri().size();
        chunkUri = _req->getUri().substr(pos, next - pos);
        int nbRoute = route->getRoutesNumber();
        int i = 0;
        while (i < nbRoute)
        {
            if (route->getRoute(i)->getRedirection() == chunkUri)
            {
                route = route->getRoute(i);
                break ;
            }
			i++;
        }
        if (i == nbRoute)
            break ;
    }
    std::string path;
    if(route == NULL)
        path = _req->getUri();
    else
    {
        if(route->getPath()[route->getPath().size() - 1] == '/')
            pos++;
        path = route->getPath() + _req->getUri().substr(pos, _req->getUri().size() - pos);
    }
    check_path(path, route);
    // genHeader("200 ok");
    return ;
}
