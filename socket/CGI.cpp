/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:16 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/25 18:26:43 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(Request& req, Server& serv, std::string path) : _req(req), _serv(serv), _path(path)
{
	
}

void	CGI::createEnv(void)
{
	_env["SERVER_SOFTWARE"] = "Webserv/1.0";
	_env["SERVER_NAME"] = _serv.getID();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = _serv.getPort();
	_env["REQUEST_METHOD"] = _req.getMethod();
	_env["PATH_INFO"] = _req.getUri();
	_env["PATH_TRANSLATED"] = _req.getUri() + _req.getQuery();
	_env["SCRIPT_NAME"] = _path;
	_env["QUERY_STRING"] = _req.getQuery();
	_env["REMOTE_HOST"] = _req.getHost();
	_env["REMOTE_ADDR"] = "";
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_USER"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = _req.getContentType();
	_env["CONTENT_LENGTH"] = _req.getContentLength();
}

char ** CGI::stringToChar(void)
{
	char** cenv = (char**)malloc((sizeof(char*) * _env.size()) + 1);
	int i = 0;

	if (cenv == NULL)
		Print::print(ERROR, "Error malloc in cgi");
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
		cenv[i++] = (char*)(it->first + "=" + it->second).c_str();
	return (cenv);
}

void	CGI::exec(void)
{
	pid_t pid;
	char **cenv = stringToChar();
	char *script = (char*) _path.c_str();

	pid = fork();
	if (pid < 0)
	{
		std::cout << "fork error\n";
		return ;
	}
	if (!pid)
	{
		execve(script, NULL, cenv);
		free(cenv);
		std::cout << "child\n";
	}
	else
	{
		free(cenv);
		std::cout << "parent\n";
	}
}

void	CGI::handler(void)
{
	if (_path.substr(_path.find_last_of('.')) == ".php")
		_script = "/usr/bin/php";
	else if (!_path.substr(_path.find_last_of('.')).compare(".py"))
		_script = "/usr/bin/python3";
}