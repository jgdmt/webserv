/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:16 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/01 17:51:07 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "../parsing/Settings.hpp"

CGI::CGI(Client* cli) : _client(cli)
{

}

CGI::CGI(CGI const& cpy)
{
	*this = cpy;
}

CGI& CGI::operator=(CGI const& other)
{
		this->_env = other._env;
		this->_client = other._client;
		this->_answer = other._answer;
		this->_end = other._end;
		return *this;
}

void	CGI::createEnv(Route* route, std::string path)
{
	std::cout << _client << "\n";
	_env["DOCUMENT_ROOT"] = route->getPath();
	// _env["SERVER_SOFTWARE"] = "Webserv";
	// _env["SERVER_NAME"] = "127.0.0.1";
	// _env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// _env["SERVER_PROTOCOL"] = "HTTP/1.1";
	// _env["SERVER_PORT"] = to_string(_serv.getPort());
	// _env["REQUEST_METHOD"] = _req.getMethod();
	// _env["PATH_INFO"] = _path;
	// _env["PATH_TRANSLATED"] = _req.getUri() + _req.getQuery();
	// _env["SCRIPT_NAME"] = _path.substr(_path.find(_route.getPath()) + _route.getPath().size() + 1);
	_env["SCRIPT_FILENAME"] = path;
	_env["QUERY_STRING"] = _client->getQuery();
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

void	CGI::body(int id)
{
	char buff[READSIZE];
	for (int i = 0; i < READSIZE; i++)
		buff[i] = 0;
	int rd = read(_end, buff, READSIZE);
	
	std::cout << "body\n";
	if (rd < 0)
		Print::print(CRASH, "Cgi has crashed so we did it too");
	else if (rd < READSIZE)
	{
		_answer.append(buff);
		if (_answer.find("Content-length") == std::string::npos)
		{
			size_t find = _answer.find("\r\n\r\n");
			size_t i = _answer.substr(find + 4).length();
			if (find == std::string::npos)
				i = _answer.length();
			_answer.insert(0, "Content-length: " + to_string(i) + "\r\n");
		}
		close(_end);
		_client->addBuffer(_answer);
		_client->_settingsPtr->getFds()->erase(_client->_settingsPtr->getFds()->begin() + id);
		_client->_settingsPtr->getFds()->at(_client->getId() + _client->_settingsPtr->getServers()->size()) = (pollfd) {_client->getFd(), POLLOUT, 0};
		_client->_settingsPtr->getCgi()->erase((_client->_settingsPtr->getCgi()->begin() + id) - (_client->_settingsPtr->getServers()->size()) - (_client->_settingsPtr->getClients()->size()));
	}
	else
		_answer.append(buff);
	// int sum = read(_end, buff, READSIZE);
	// 	std::cout << buff << "\n";
	// while (sum != 0)
	// {
	// 	_answer.append(buff);
	// 	break ;
	// }
	// if (_answer.find("Content-length") == std::string::npos)
	// {
	// 	size_t i = (_answer.substr(_answer.find("\r\n\r\n") + 4)).size();
	// 	_answer.insert(0, "Content-length: " + to_string(i) + "\r\n");
	// }
}

void	CGI::exec(char **script)
{
	pid_t pid;
	char **cenv = stringToChar();
	int end[2];
	
	if (pipe(end) == -1)
	{
		std::cout << "Pipe error\n";
		return ;
	}
	_end = end[0];
	_client->_settingsPtr->getFds()->push_back((pollfd){_end, POLLIN, 0});
	pid = fork();
	if (pid < 0)
	{
		std::cout << "fork error\n";
		close(end[0]);
		close(end[1]);
		_client->_settingsPtr->getFds()->pop_back();
		return ;
	}
	else if (!pid)
	{
		dup2(end[1], STDOUT_FILENO);
		// // dup2(_end[0], STDIN_FILENO);
		close(end[0]);
		close(end[1]);
		execve(script[0], script, cenv);
	}
	else
	{
		close(end[1]);
	}
}

void	CGI::handler(Route* route, std::string path)
{
	std::string s = route->getCgiPath();
	std::cout << s << "\n";
	char *script[3];

	_answer.clear();
	script[0] = (char*) s.c_str();
	script[1] = (char*) path.c_str();
	script[2] = NULL;
	createEnv(route, path);
	exec(script);
}
