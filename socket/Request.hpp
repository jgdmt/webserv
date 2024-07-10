/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 18:31:39 by vilibert         ###   ########.fr       */
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
	TRANSFER_ENCODING,
    COOKIE,
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
	bool transfer_encoding;
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
        std::string const &getCookies(void) const;
		size_t const& getContentLength(void) const;
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
        std::string _cookies;
        size_t _contentLength;
        std::vector<std::string> _accept;
		std::vector<std::string> _acceptEncoding;
		std::vector<std::string> _transferEncoding;
        enum State _state;
        bool _error;
		bool _chunked;
		t_headerStatus _headerStatus;

    private:
       int parseHeader(void);
       void parseBody(void);
       void setAccept(std::string const& line);
	   void	acceptEncoding(std::string const& line);
	   void transferEncoding(std::string const& line);

		template <typename T>
		static void swap(T& a, T& b)
		{
			T	c = a;

			a = b;
			b = c;
		}
};