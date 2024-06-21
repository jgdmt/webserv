/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/21 11:49:53 by jgoudema         ###   ########.fr       */
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
        std::string const &getConnection(void);
        std::string const &getUri(void) const;
		std::string const &getMethod(void) const;
		std::string const &getHost(void) const;
		std::string const &getContentType(void) const;
		std::string const &getContentBoundary(void) const;
		std::string const &getBody(void) const;
		std::string const &getAccept(int i) const;
		std::string const &getAcceptEncoding(int i) const;
		uint32_t const& getContentLength(void) const;
		size_t getAcceptSize(void);
		size_t getAcceptEncodingSize(void);


        int IsParsingOk(void);
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
		std::vector<std::string> _acceptEncoding;
        enum State _state;
        bool _error;
      
	  
       int parseHeader(void);
       int parseBody(void);
       void accept(std::string const& line);
	   void	acceptEncoding(std::string const& line);

		template <class T>
		static T convertType(std::string entry)
		{
			T s = 0;
			std::stringstream	ss(entry);

			ss >> s;
			return (s);
		}

		template <typename T>
		static void swap(T& a, T& b)
		{
			T	c = a;

			a = b;
			b = c;
		}
};