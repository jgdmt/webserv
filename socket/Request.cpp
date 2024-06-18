/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/18 12:06:15 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"



Request::Request()
{
    _state = METHOD;
    _error = 0;
    _contentLength = -1;
}

static int getParam(std::string const &param)
{
    std::string params[] = {"Host", "Connection", "Accept", "Accept-Encoding", "Content-Type", "Content-Length", "end"};
    for(int i = 0; params[i] != "end"; i++)
        if (params[i] == param)
            return (i);
    if (param.size() == 0)
        return (-1);
    else
        return (OTHER);
}

// void Request::startLine()
// {
//     std::string::iterator it2 = _buffer.begin() + _buffer.find(' ');

//     _method = std::string(_it, it2);
//     _it = it2;
//     it2 = _buffer.begin() + _buffer.find(' ', _it - _buffer.begin());
//     _uri = std::string(_it, it2);
//     _it = it2;
//     it2 = _buffer.begin() + _buffer.find(' ', _it - _buffer.begin());
//     if (std::string(_it, it2) != )
// }

int Request::parseHeader(void)
{
    std::string line;

    if (_buffer.find('\n', _it - _buffer.begin()) == std::string::npos)
        return (1);
    line = std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()));
    if (line.find(": ") == std::string::npos)
    {
         _error = 1;
        return (1);
    }
    int t = getParam(line.substr(0, line.find(':')));
    _it =  _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) + 1;
    while(t != EMPTY)
    {
        switch (t)
        {
            case HOST:
                _host = std::string(line.begin() + line.find(": ") + 2, line.end());
                std::cout << "host:" << _host << "\n";
                break;
            case CONNECTION:
                _connection = std::string(line.begin() + line.find(": ") + 2, line.end());
                break;
            case ACCEPT:
                break;
            case ACCEPT_ENCODING:
                break;
            case CONTENT_TYPE:
                _contentType = std::string(line.begin() + line.find(": ") + 2, line.begin() + line.find(';'));
                break; // add
            case CONTENT_LENGTH:
                _contentLength = convertType<uint32_t>(std::string(line.begin() + line.find(": ") + 2, line.end()));
            default:
                break;
        }
        if (_buffer.find("\r\n", _it - _buffer.begin()) == (size_t)(_it - _buffer.begin()))
            break ;
        if (_buffer.find('\n', _it - _buffer.begin()) == std::string::npos)
            return (1);
        line = std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()));
        if (line.find(": ") == std::string::npos)
        {
            _error = 1;
            std::cout << "error\n";
            return (1);
        }
        t = getParam(line.substr(0, line.find(": ")));
        _it =  _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) + 1;
    }
    _state = BODY;
    return (0);
}

void Request::add(std::string const &new_buff)
{
    _buffer += new_buff;

    //parse starting line
    switch (_state)
    {
        case METHOD:
            _it = _buffer.begin();
            if(_buffer.find(' ') == std::string::npos)
                break;
            _method = std::string(_it, _buffer.begin() + _buffer.find(' '));
            _it = _buffer.begin() + _buffer.find(' ') + 1;
            _state = URI;
        case URI:
            if(_buffer.find(' ', _it - _buffer.begin()) == std::string::npos)
                break;
            _uri = std::string(_it, _buffer.begin() + _buffer.find(' ', _it - _buffer.begin()));
            _it = _buffer.begin() + _buffer.find(' ', _it - _buffer.begin()) + 1;
            _state = PROTOCOL;
        case PROTOCOL:
            if(_buffer.find('\n', _it - _buffer.begin()) == std::string::npos)
                break;
            if ("HTTP/1.1" != std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin())))
                _error = true;
            _it = _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) + 1;
            _state = HEADER;
        case HEADER:
            parseHeader();
        case BODY:
            
            break;
        case END:
            break;
    }
    //else body
}


int Request::getState(void)
{
    return _state;
    // std::cout << _buffer;
   
    
    // return 1;
}