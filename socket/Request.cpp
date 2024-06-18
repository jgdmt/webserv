/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/18 17:38:46 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"



Request::Request()
{
    _state = METHOD;
    _error = 0;
    _contentLength = 0;
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

void Request::accept(void)
{
    
}

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
                accept();
                break;
            case ACCEPT_ENCODING:
                break;
            case CONTENT_TYPE:
                if (line.find("; boundary=") != std::string::npos)
                {
                    _contentType = std::string(line.begin() + line.find(": ") + 2, line.begin() + line.find(';'));
                    _contentBoundary = std::string(line.begin() + line.find("; boundary=") + 11, line.end());
                }
                else
                    _contentType = std::string(line.begin() + line.find(": ") + 2, line.end());
                break; // add
            case CONTENT_LENGTH:
                _contentLength = convertType<uint32_t>(std::string(line.begin() + line.find(": ") + 2, line.end()));
            default:
                break;
        }
        if (_buffer.find("\r\n", _it - _buffer.begin()) == (size_t)(_it - _buffer.begin()))
        {
            _it += 2;
            break ;
        }
        if (_buffer.find('\n', _it - _buffer.begin()) == std::string::npos)
            return (1);
        line = std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()));
        if (line.find(": ") == std::string::npos)
        {
            _error = 1;
            return (1);
        }
        t = getParam(line.substr(0, line.find(": ")));
        _it =  _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) + 1;
    }
    _state = BODY;
    return (0);
}

int Request::parseBody(void)
{
    std::string tmp(_it, _buffer.end());

    if(_contentLength == 0)
    {
        _state = END;
        return 0;
    }
    if (tmp.size() + _body.size() >= _contentLength)
    {
        _body += tmp.substr(0, _contentLength - _body.size());
        _state = END;
        return (0);
    }
    else
    {
        _body += tmp;
        _it += tmp.size(); 
    }
    return (1);
}

void Request::add(std::string const &new_buff)
{
    _buffer += new_buff;
    std::cout << _buffer;
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
            if ("HTTP/1.1" != std::string(_it, _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) - 1))
                _error = true;
            _it = _buffer.begin() + _buffer.find('\n', _it - _buffer.begin()) + 1;
            _state = HEADER;
        case HEADER:
            if (parseHeader())
                break;
        case BODY:
            if (parseBody())
            break;
        case END:
            break;
    }
    //else body
}


int Request::IsParsingOk(void)
{
    if (_error)
        return -1;
    else if (_state != END)
        return 0;
    else if ((!_method.size() || !_uri.size() || !_host.size() || !_host.size()))
        return -1;
    else if ( _contentLength == 0 && (_method == "POST"))
        return -2;
    else
        return 1;
}