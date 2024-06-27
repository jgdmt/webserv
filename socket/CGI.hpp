/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/27 19:58:35 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include "Request.hpp"
#include "Client.hpp"
#include "../parsing/Server.hpp"
#include "../Print.hpp"

class CGI {
	public:
		CGI(Request &req, Server &serv, Route& route, std::string path);
		CGI(CGI const& cpy);
		std::string handler(void);
	private:
		std::map<std::string, std::string> _env;
		Request& _req;
		Server& _serv;
		Route& _route;
		std::string _path;
		std::string _answer;
		char *_script[3];
		int	_end[2];

		char** stringToChar();
		void createEnv(void);
		void exec(void);
		void body(void);
};