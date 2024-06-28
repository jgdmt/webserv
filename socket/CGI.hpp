/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/28 15:46:05 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include "Request.hpp"
#include "../parsing/Server.hpp"
#include "../Print.hpp"

#define READSIZE 50000

class Settings;

class CGI {
	public:
		CGI(Request *req, Server *serv, Settings *settings);
		CGI(CGI const& cpy);
		CGI& operator=(CGI const& other);
		std::string handler(Route* route, std::string path);
	private:
		std::map<std::string, std::string> _env;
		Request* _req;
		Server* _serv;
		Settings* _settings;
		std::string _answer;
		int	_end;

		char** stringToChar(void);
		void createEnv(Route* route, std::string path);
		void exec(char ** script);
		void body(void);
};