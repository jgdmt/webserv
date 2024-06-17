/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/17 19:24:52 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

enum Paramater 
{
    EMPTY = -1,
    HOST = 0,
    CONNECTION,
    ACCEPT,
    ACCEPT_ENCODING,
    OTHER,
};

Request::Request()
{
    _state = METHOD;
    _error = 0;
}

static int getParam(std::string const &param)
{
    std::cout << "PARAM" << param << "\n";
    std::string params[] = {"Host", "Connection", "Accept", "Accept-Encoding", "end"};
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
    _it =  _buffer.begin() + _buffer.find('\n', _it - _buffer.begin());
    while(t != EMPTY)
    {
        switch (t)
        {
            case HOST:
                // this->_host
                break;
            case CONNECTION:
                break;
            case ACCEPT:
                break;
            case ACCEPT_ENCODING:
                break;
            default:
                break;
        }
        if (_buffer.find('\n', _it - _buffer.begin()) == std::string::npos)
            return (1);
        line = std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()));
        if (line.find(": ") == std::string::npos)
        {
            _error = 1;
            return (1);
        }
        t = getParam(line.substr(0, line.find(':')));
        _it =  _buffer.begin() + _buffer.find('\n', _it - _buffer.begin());
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
    }
    //else body
}


bool Request::parse(void)
{
    std::cout << _buffer;
   
    
    return 1;
}