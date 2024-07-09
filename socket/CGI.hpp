/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:10 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/09 15:58:08 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "Request.hpp"
#include "../parsing/Server.hpp"
#include "../Print.hpp"

class Settings;

class CGI {
	public:
		CGI(std::vector<Client>::iterator cli, int id);
		CGI(CGI const& cpy);
		CGI& operator=(CGI const& other);
		int	getID();
		void setClient(std::vector<Client>::iterator cli);
		std::vector<Client>::iterator getClient(void);
		void setID(int id);
		void handler(Route* route, std::string path);
		void body(int id);
		time_t const &getStartTime() const;
		void	closeCGI(int id);

	private:
		std::map<std::string, std::string> _env;
		std::vector<Client>::iterator _client;
		int _clientID;
		std::string _answer;
		int	_end;
		pid_t _pid;
		time_t _startTime;

		char** stringToChar(void);
		void createEnv(std::string path);
		void exec(char ** script);
};