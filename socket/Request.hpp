/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/18 13:23:02 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Print.hpp"
#include "../Utils.hpp"
#include <vector>

enum State 
{
    METHOD = 0,
    URI,
    PROTOCOL,
    HEADER,
    BODY,
    END,
};

enum Paramater 
{
    EMPTY = -1,
    HOST = 0,
    CONNECTION,
    ACCEPT,
    ACCEPT_ENCODING,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    OTHER,
};

class Request
{
    public:
        Request();
        void add(std::string const &new_buff);
        int getState(void);
    private:
        std::string::iterator _it;
        std::string _buffer;
        std::string _method;
        std::string _uri;
        std::string _host;
        std::string _connection;
        std::string _contentType;
        std::string _contentBoundary;
        std::string _body;
        uint32_t _contentLength;
        std::vector<std::string> _accept;
        enum State _state;
        bool _error;
      
	  
       int parseHeader(void);
       int parseBody(void);
       void accept(void);
};