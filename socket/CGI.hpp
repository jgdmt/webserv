/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/01 19:34:45 by jgoudema         ###   ########.fr       */
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

class Settings;

class CGI {
	public:
		CGI(Client *cli);
		CGI(CGI const& cpy);
		CGI& operator=(CGI const& other);
		void handler(Route* route, std::string path);
		void body(int id);
	private:
		std::map<std::string, std::string> _env;
		Client* _client;
		std::string _answer;
		int	_end;
		pid_t _pid;

		char** stringToChar(void);
		void createEnv(Route* route, std::string path);
		void exec(char ** script);
};