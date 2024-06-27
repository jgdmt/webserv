/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/26 17:19:51 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include "Request.hpp"
#include "../parsing/Server.hpp"
#include "../Print.hpp"
class CGI {
	public:
		CGI(Request &req, Server &serv, Route& route, std::string path);
		CGI(CGI const& cpy);
		void handler(void);
	private:
		std::map<std::string, std::string> _env;
		Request& _req;
		Server& _serv;
		Route& _route;
		std::string _path;
		char *_script[3];

		char** stringToChar();
		void createEnv(void);
		void exec(void);
};