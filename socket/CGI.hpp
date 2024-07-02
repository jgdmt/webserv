/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/02 15:55:44 by vilibert         ###   ########.fr       */
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
		CGI(std::vector<Client>::iterator cli);
		CGI(CGI const& cpy);
		CGI& operator=(CGI const& other);
		void handler(Route* route, std::string path);
		void body(int id);
	private:
		std::map<std::string, std::string> _env;
		std::vector<Client>::iterator _client;
		std::string _answer;
		int	_end;
		pid_t _pid;

		char** stringToChar(void);
		void createEnv(Route* route, std::string path);
		void exec(char ** script);
};