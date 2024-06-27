/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:16 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/27 20:46:25 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(Request& req, Server& serv, Route& route, std::string path) : _req(req), _serv(serv), _route(route), _path(path)
{
	_script[1] = NULL;
}

void	CGI::createEnv(void)
{
	// _env["DOCUMENT_ROOT"] = _route.getPath();
	// _env["SERVER_SOFTWARE"] = "Webserv";
	// _env["SERVER_NAME"] = "127.0.0.1";
	// _env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// _env["SERVER_PROTOCOL"] = "HTTP/1.1";
	// _env["SERVER_PORT"] = to_string(_serv.getPort());
	// _env["REQUEST_METHOD"] = _req.getMethod();
	// _env["PATH_INFO"] = _path;
	// _env["PATH_TRANSLATED"] = _req.getUri() + _req.getQuery();
	// _env["SCRIPT_NAME"] = _path.substr(_path.find(_route.getPath()) + _route.getPath().size() + 1);
	// _env["SCRIPT_FILENAME"] = _path;
	_env["QUERY_STRING"] = _req.getQuery();
	// _env["REMOTE_ADDR"] = _req.getHost();
	// _env["AUTH_TYPE"] = "Basic";
	// _env["CONTENT_TYPE"] = _req.getContentType();
	// _env["CONTENT_LENGTH"] = _req.getContentLength();
	//_env["HTTP_COOKIE"] = ;

}

char ** CGI::stringToChar(void)
{
	char** cenv = (char**)malloc(sizeof(char*) * (_env.size() + 1));
	int i = 0;
	std::string s;
	if (cenv == NULL)
		Print::print(ERROR, "Error malloc in cgi");
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		s = it->first;
		s.append("=");
		s.append(it->second);
		cenv[i++] = strdup((char*) s.c_str());
	}
	cenv[i] = NULL;
	return (cenv);
}

void	CGI::body(void)
{
	char buff[READSIZE];
    // std::tm *time = std::localtime(&now);
    // char buff[80];

    // _buffer.clear();
    // _buffer = "HTTP/1.1 " + type + "\r\n";
    // if (_req->getConnection() == "keep-alive\r")
    // {
    //     _buffer.append("Connection: Keep-Alive\r\n");
    //     _buffer.append("Keep-Alive: timeout=60\r\n");
    // }
    // strftime(buff, 80, "%c", time);
    // _buffer.append("Date: " + (std::string)buff + "\r\n");
    // _buffer.append("Server: IsWatchingYou\r\n");
	int sum = read(_end[0], buff, READSIZE);
	// std::cout << sum <<"\n";
	while (sum != 0)
	{
		_answer.append(buff);
		// int i = 0;
		// while(buff[i]) i++;
		// std::cout << i << "\n";
		// i =-1;
		// while (buff[++i])// && buff[i] != '\n')
		// 	std::cout << i << " " << buff[i];
	// 	std::cout << i << " | "  << buff[i - 2]<< "\n";
	// }
		// sum = read(_end[0], buff, READSIZE);
		// std::cout << sum << "\n";
		break ;
	}
		// std::cout << "ANSWER" << _answer << "\n";
}

void	CGI::exec(void)
{
	pid_t pid;
	char **cenv = stringToChar();

	if (pipe(_end) == -1)
	{
		std::cout << "Pipe error\n";
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		std::cout << "fork error\n";
		return ;
	}
	if (!pid)
	{
		dup2(_end[1], STDOUT_FILENO);
		// dup2(_end[0], STDIN_FILENO);
		close(_end[0]);
		close(_end[1]);
		execve(_script[0], _script, cenv);
	}
}

std::string	CGI::handler(void)
{
	std::string s;
	if (_path.substr(_path.find_last_of('.')) == ".php")
		s = "/usr/bin/php";
	else if (!_path.substr(_path.find_last_of('.')).compare(".py"))
		s = "/usr/bin/python3";
	_script[0] = (char*) s.c_str();
	_script[1] = (char*) _path.c_str();
	_script[2] = NULL;
	createEnv();
	exec();
	body();
	return (_answer);
	// 	pid_t pid;
	// char **cenv = stringToChar();

	// pid = fork();
	// if (pid < 0)
	// {
	// 	std::cout << "fork error\n";
	// 	return ;
	// }
	// if (!pid)
	// {
	// 	std::cout << _script[0] << "\n" << _script[1] << "\n";
	// 	for (size_t i = 0; cenv[i]; i++)
	// 		std::cout << cenv[i] << "\n";
	// 	execve(_script[0], _script, cenv);
	// }


/*	char *argv[3];
	char *env[20];

	argv[0] = strdup("/usr/bin/python3");
argv[1] = strdup("cgi-bin/get_hello.py");
argv[2] = NULL;
// env[0] = strdup("AUTH_TYPE=Basic");
// env[1] = strdup("CONTENT_LENGTH=");
// env[2] = strdup("CONTENT_TYPE=");
// env[1] = strdup("DOCUMENT_ROOT=./");
// env[0] = strdup("GATEWAY_INTERFACE=CGI/1.1");
// env[5] = strdup("HTTP_COOKIE=");
// env[6] = strdup("PATH_INFO=");
// env[7] = strdup("PATH_TRANSLATED=.//");
env[0] = strdup("QUERY_STRING=first_name=&last_name=");
// env[9] = strdup("REDIRECT_STATUS=200");
// env[8] = strdup("REMOTE_ADDR=127.0.0.1:8002");
// env[8] = strdup("REQUEST_METHOD=GET");
// env[9] = strdup("REQUEST_URI=/cgi-bin/get_hello.pyfirst_name=&last_name=");
// env[10] = strdup("SCRIPT_FILENAME=get_hello.py");
// env[11] = strdup("SCRIPT_NAME=cgi-bin/get_hello.py");
// env[12] = strdup("SERVER_NAME=127.0.0.1");
// env[13] = strdup("SERVER_PORT=8002");
// env[14] = strdup("SERVER_PROTOCOL=HTTP/1.1");
// env[15] = strdup("SERVER_SOFTWARE=AMANIX");
env[1] = NULL;
	int pid = fork();

	if (!pid)
	{
		execve(argv[0], argv, env);
	}
*/
}
