/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:16 by jgoudema          #+#    #+#             */
/*   Updated: 2024/07/08 18:57:22 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "../parsing/Settings.hpp"

CGI::CGI(std::vector<Client>::iterator cli, int id): _client(cli), _clientID(id) 
{

}

CGI::CGI(CGI const& cpy)
{
	*this = cpy;
}

int	CGI::getID()
{
	return _clientID;
}

void CGI::setClient(std::vector<Client>::iterator cli)
{
	_client = cli;
}
void CGI::setID(int id)
{
	_clientID = id;
}

CGI& CGI::operator=(CGI const& other)
{
		this->_env = other._env;
		this->_client = other._client;
		this->_clientID = other._clientID;
		this->_answer = other._answer;
		this->_end = other._end;
		return *this;
}

void	CGI::createEnv(Route* route, std::string path)
{
	char buffer[INET_ADDRSTRLEN];
	_env["AUTH_TYPE"] = "";
	_env["CONTENT_LENGTH"] = _client->getContentLength();
	_env["CONTENT_TYPE"] = _client->getContentType();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = path;
	_env["PATH_TRANSLATED"] = _client->getUri() + _client->getQuery();
	_env["QUERY_STRING"] = _client->getQuery();
	_env["REMOTE_ADDR"] = inet_ntop(AF_INET, _client->getAddr(), buffer, INET_ADDRSTRLEN);
	_env["REQUEST_METHOD"] = _client->getMethod();
	_env["SCRIPT_NAME"] = path.substr(path.find(route->getPath()) + route->getPath().size() + 1);
	_env["SERVER_NAME"] = _client->getHost();
	_env["SERVER_PORT"] = to_string(_client->_serverPtr->getPort());
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "IsWatchingYou";
	_env["HTTP_COOKIE"] = _client->getCookies();

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
	int status;
	char buff[READSIZE + 1];
	// static int st = 0;
	for (int i = 0; i <= READSIZE; i++)
		buff[i] = 0;
	int rd = read(_end, buff, READSIZE);

	if (rd < 0)
	{
		Print::print(ERROR, "CGI: pipe read failed");
		_client->error("500", "Internal Server Error");
		_client->_settingsPtr->getFds()->erase(_client->_settingsPtr->getFds()->begin() + id);
		_client->_settingsPtr->getFds()->at(_client->getId() + _client->_settingsPtr->getServers()->size()) = (pollfd) {_client->getFd(), POLLOUT, 0};
		_client->_settingsPtr->getCgi()->erase((_client->_settingsPtr->getCgi()->begin() + id) - (_client->_settingsPtr->getServers()->size()) - (_client->_settingsPtr->getClients()->size()));
	}
	else if (rd == 0)
	{
		// _answer.append(buff);
		std::cout << _answer << "\n";
		if (_answer.find("Content-length") == std::string::npos)
		{
			size_t find = _answer.find("\r\n\r\n");
			if (find == std::string::npos)
			{
				_client->error("500", "Internal Server Error");
			}
			else
				_answer.insert(0, "Content-length: " + to_string(_answer.substr(find + 4).length()) + "\r\n");
		}
		close(_end);
		waitpid(_pid, &status, 0);
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			_client->error("500", "Internal Server Error");
		else if ( _answer.find("\r\n\r\n") != std::string::npos)
			_client->addBuffer(_answer);
		_client->_settingsPtr->getFds()->erase(_client->_settingsPtr->getFds()->begin() + id);
		_client->_settingsPtr->getFds()->at(_client->getId() + _client->_settingsPtr->getServers()->size()) = (pollfd) {_client->getFd(), POLLOUT, 0};
		_client->_settingsPtr->getCgi()->erase((_client->_settingsPtr->getCgi()->begin() + id) - (_client->_settingsPtr->getServers()->size()) - (_client->_settingsPtr->getClients()->size()));
	
	}
	else
	{
		// if (st == 0)
		// 	std::cout << buff << "\n";
		_answer.append(buff);
		// st++;
	}
}

void	CGI::exec(char **script)
{
	char **cenv = stringToChar();
	int end[2];
	
	if (pipe(end) == -1)
	{
		std::cout << "Pipe error\n";
		return ;
	}
	_end = end[0];
	_client->_settingsPtr->getFds()->push_back((pollfd){_end, POLLIN, 0});
	_pid = fork();
	if (_pid < 0)
	{
		std::cout << "fork error\n";
		close(end[0]);
		close(end[1]);
		_client->_settingsPtr->getFds()->pop_back();
		return ;
	}
	else if (!_pid)
	{
		dup2(end[1], STDOUT_FILENO);
		// // dup2(_end[0], STDIN_FILENO);
		close(end[0]);
		close(end[1]);
		if(chdir(((std::string)script[1]).substr(0, ((std::string)script[1]).find_last_of("/")).c_str()) == -1)
			exit(1);
		script[1] = (char*)((std::string)script[1]).substr(((std::string)script[1]).find_last_of("/") + 1).c_str();
		if (execve(script[0], script, cenv) == -1)
			Print::print(ERROR, "Execve failed");
	}
	else
	{
		close(end[1]);
	}
}

void	CGI::handler(Route* route, std::string path)
{
	std::string s = route->getCgiPath();
	char *script[3];

	_answer.clear();
	script[0] = (char*) s.c_str();
	script[1] = (char*) path.c_str();
	script[2] = NULL;
	createEnv(route, path);
	exec(script);
}
