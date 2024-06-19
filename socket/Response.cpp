/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/19 16:28:39 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request* req, Server* serv)
{
    _req = req;
    _serv = serv;
}

void Response::genHeader(std::string type)
{
    std::time_t now = std::time(nullptr);
    std::tm *time =  std::localtime(&now);
    char buff[80];

    _buffer += "HTTP/1.1 " + type + "\r\n";
    if (_req->getConnection() == "keep-alive\r")
        _buffer += "Connection: Keep-Alive\r\n";
    strftime(buff, 80, "%c", time);
    _buffer += "Date: " + (std::string)buff + "\r\n";
    _buffer += "Keep-Alive: timeout=60\r\n";
    _buffer += "Server: IsWatchingYou\r\n";
    std::cout << _buffer << "\n";
}
// static std::string getMimeType(const string &szExtension)
// {
//     // return mime type for extension
//     HKEY hKey = NULL;
//     string szResult = "application/unknown";

//     // open registry key
//     if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension.c_str(), 
//                        0, KEY_READ, &hKey) == ERROR_SUCCESS)
//     {
//         // define buffer
//         char szBuffer[256] = {0};
//         DWORD dwBuffSize = sizeof(szBuffer);

//         // get content type
//         if (RegQueryValueEx(hKey, "Content Type", NULL, NULL, 
//                        (LPBYTE)szBuffer, &dwBuffSize) == ERROR_SUCCESS)
//         {
//             // success
//             szResult = szBuffer;
//         }

//         // close key
//         RegCloseKey(hKey);
//     }

//     // return result
//     return szResult;
// }

void Response::genBody(std::string path)
{
    std::ifstream FileDescriptor(path);
    if(FileDescriptor.fail())
    {
        Print::print(ERROR, "Couldn't open body file");
        return ;
    }
    std::stringstream FileStream;
    FileStream << FileDescriptor.rdbuf();
    std::string buff = FileStream.str();
    _buffer += "Content-Length: " + to_string(buff.size()) + "\r\n";
    // _buffer += "Content-Type: " + getMimType(path.substr(path.find('.', path.find_last_of('/'))) + "\r\n\r\n");
    _buffer += buff;
    _buffer += "\r\n";
}

void Response::error(std::string httpErrorCode, std::string httpErrorMessage)
{
    genHeader(httpErrorCode + ' ' + httpErrorMessage);
    std::string path = _serv->getErrorPage(httpErrorCode);
    genBody(path);
    
    return;
}

// void Response::rec(int pos, Route * route)
// {
//     std::string chunkUri = req->getUri().substr(req->getUri().find('/', pos));

//     for (unsigned int i = 0; i < route->getRoutesNumber(); i++)
//     {
//         if (route->getRoute(i)->getRedirection() == chunkUri)
//             rec(req->getUri().find('/', pos) + 1, );
//     }
// }

void Response::init(void)
{
    size_t pos = 0;
    size_t next = _req->getUri().find('/', pos + 1);
    if (next == std::string::npos)
        next = _req->getUri().size();
    std::string chunkUri = _req->getUri().substr(pos, next - pos);
    Route *route = NULL;

        std::cout << "serv '" << chunkUri << "'\n";
    for(unsigned int i = 0; i < _serv->getRoutesNumber(); i++)
    {
        std::cout << "serv2 '" << _serv->getRoute(i)->getRedirection() << "'\n";
        if(_serv->getRoute(i)->getRedirection() == chunkUri)
        {
            route = _serv->getRoute(i);
            break ;
        }
    }
    if (route == NULL)
        std::cout << "default case\n";
    while (route != NULL)
    {
        pos = next;
        next = _req->getUri().find('/', pos + 1);
        if (next == std::string::npos)
            break;
        chunkUri = _req->getUri().substr(pos, next - pos);
        int nbRoute = route->getRoutesNumber();
        int i = 0;
        while (i < nbRoute)
        {
            if (route->getRoute(i)->getRedirection() == chunkUri)
            {
                route = route->getRoute(i);
                std::cout << route->getRedirection();
                break ;
            }
        }
        if (i == nbRoute)
            break ;
    }
    std::cout << _req->getUri() << "\n";
    // std::cout << route->getRedirection();
    std::cout << "coucou\n";
    genHeader("200 ok");
    return ;
}
