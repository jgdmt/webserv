/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/04 15:05:17 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Print.hpp"
#include "../Utils.hpp"
#include <vector>
#include <cstring>

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

typedef struct s_headerStatus
{
	bool host;
	bool connection;
	bool accept;
	bool accept_encoding;
	bool content_type;
	bool content_length;
} t_headerStatus ;

class Client;

class Request
{
    public:
        Request();
        ~Request();
        void add(std::string const &new_buff);
        void clear(void);
        std::string const &getConnection(void);
        std::string const &getUri(void) const;
		std::string const &getMethod(void) const;
		std::string const &getHost(void) const;
		std::string const &getContentType(void) const;
		std::string const &getContentBoundary(void) const;
		std::string const &getBody(void) const;
		std::string const &getAccept(int i) const;
		std::string const &getAcceptEncoding(int i) const;
		std::string const &getQuery(void) const;
		unsigned int const& getContentLength(void) const;
		size_t getAcceptSize(void);
		size_t getAcceptEncodingSize(void);

        int IsParsingOk(void);

	protected:
        size_t _i;
        std::string _buffer;
        std::string _method;
        std::string _uri;
		std::string _query;
        std::string _host;
        std::string _connection;
        std::string _contentType;
        std::string _contentBoundary;
        std::string _body;
        unsigned int _contentLength;
        std::vector<std::string> _accept;
		std::vector<std::string> _acceptEncoding;
        enum State _state;
        bool _error;
		t_headerStatus _headerStatus;

    private:
       int parseHeader(void);
       int parseBody(void);
       void setAccept(std::string const& line);
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