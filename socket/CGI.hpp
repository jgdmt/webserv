/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/01 14:21:16 by vilibert         ###   ########.fr       */
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
		CGI(Client *cli, Server *serv, Settings *settings);
		CGI(CGI const& cpy);
		CGI& operator=(CGI const& other);
		void handler(Route* route, std::string path);
		void body(int id);
	private:
		std::map<std::string, std::string> _env;
		Server* _serv;
		Settings* _settings;
		Client* _client;
		std::string _answer;
		int	_end;
		int	_id;

		char** stringToChar(void);
		void createEnv(Route* route, std::string path);
		void exec(char ** script);
};