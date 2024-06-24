/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/24 14:39:55 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"



Request::Request()
{
    _state = METHOD;
    _error = 0;
    _contentLength = 0;
}

std::string const &Request::getConnection(void)
{
    return _connection;
}

std::string const &Request::getUri() const
{
    return _uri;
}

std::string const &Request::getMethod(void) const
{
	return _method;
}

std::string const &Request::getHost(void) const
{
	return _host;
}

std::string const &Request::getContentType(void) const
{
	return _contentType;
}

std::string const &Request::getContentBoundary(void) const
{
	return _contentBoundary;
}

std::string const &Request::getBody(void) const
{
	return _body;
}

std::string const &Request::getAccept(int i) const
{
	return _accept[i];
}

std::string const &Request::getAcceptEncoding(int i) const
{
	return _acceptEncoding[i];
}

unsigned int const &Request::getContentLength(void) const
{
	return _contentLength;
}

size_t Request::getAcceptSize(void)
{
	return _accept.size();
}

size_t Request::getAcceptEncodingSize(void)
{
	return _acceptEncoding.size();
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

void Request::accept(std::string const& line)
{
    size_t i = line.find(": ") + 2; //eventuellement verif si ':' existe
	size_t j;
	size_t k;
	std::vector<float> sort;

	while (1)
	{
		j = line.find(",", i);
		if (j == std::string::npos)
			j = line.size() - 1;
		k = line.find(";q=", i);
		if (k < j)
		{
			_accept.push_back(line.substr(i, k - i));
			sort.push_back(convertType<float>(line.substr(k + 3, j - k)));
		}
		else
		{
			_accept.push_back(line.substr(i, j - i));
			sort.push_back(1);
		}
		if (j == line.size() - 1)
			break ;
		i = j + 1;
	}

	size_t min = 0;
	for (size_t j = 0; j < sort.size(); j++)
	{
		for (size_t i = j; i < sort.size(); i++)
		{
			if (sort[i] < sort[min])
				min = i;
		}
		swap<float>(sort[j], sort[min]);
		swap<std::string>(_accept[j], _accept[min]);
	}

	// for (size_t i = 0; i < sort.size(); i++)
	// {
	// 	std::cout << "accept [" << sort[i] << "] -> " << _accept[i] << "\n";
	// }
}

void Request::acceptEncoding(std::string const& line)
{
	size_t i = line.find(": ") + 2; // eventuellement verifier si ':' existe
	size_t j;

	while (1)
	{
		j = line.find(", ", i);
		if (j == std::string::npos)
		{
			j = line.size() - 1;
			_acceptEncoding.push_back(line.substr(i, j - i));
			break;
		}
		_acceptEncoding.push_back(line.substr(i, j - i));
		i = j + 2;
	}
	
	// for (size_t i = 0; i < _acceptEncoding.size(); i++)
	// 	std::cout << "acceptEncoding[" << i << "] = " << _acceptEncoding[i] << "\n";
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
                break;
            case CONNECTION:
                _connection = std::string(line.begin() + line.find(": ") + 2, line.end());
                break;
            case ACCEPT:
                accept(line);
                break;
            case ACCEPT_ENCODING:
				acceptEncoding(line);
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
                _contentLength = convertType<unsigned int>(std::string(line.begin() + line.find(": ") + 2, line.end()));
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

void Request::clear(void)
{
    _buffer.clear();
}

void Request::add(std::string const &new_buff)
{
    _buffer += new_buff;
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
    else if (!_method.size() || !_uri.size() || !_host.size())
        return -1;
    else if ( _contentLength == 0 && (_method == "POST"))
        return -2;
return 1;
}