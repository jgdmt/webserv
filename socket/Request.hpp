/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/17 19:14:21 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Print.hpp"

enum State {
    METHOD = 0,
    URI,
    PROTOCOL,
    HEADER,
    BODY,
};

class Request
{
    public:
        Request();
        void add(std::string const &new_buff);
        bool parse(void);
    private:
        std::string::iterator _it;
        std::string _buffer;
        std::string _host;
        std::string _connection;
        std::string _method;
        std::string _uri;
        enum State _state;
        bool _error;
      
	  
       void startLine(void);
       int parseHeader(void);
};