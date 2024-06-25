/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/25 16:05:21 by jgoudema         ###   ########.fr       */
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
		CGI(Request &req, Server &serv, std::string path);
		CGI(CGI const& cpy);
		void handler(void);
	private:
		std::map<std::string, std::string> _env;
		Request& _req;
		Server& _serv;
		std::string _path;
		std::string _script;

		char** stringToChar();
		void createEnv(void);
		void exec(void);
};