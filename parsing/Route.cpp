/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:32:07 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/25 15:53:16 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route()
{
    _path = "default/web";
    _directoryListing = true;
    _defaultFileForDirectory = "default/directory.html";
}

bool Route::_isAutorizedMethod(std::string const &method)
{
    if(find(_autorizedMethods.begin(), _autorizedMethods.end(), method) != _autorizedMethods.end())
        return true;
    else
        return false;
}

std::string const &Route::getRedirection(void) const
{
    return _redirection;
}

std::string const &Route::getPath(void) const
{
    return _path;
}

std::string const &Route::getCgiPath(void) const
{
	return _cgiPath;
}

std::string const &Route::getCgiExtension(int i) const
{
	return _cgiExtensions[i];
}

size_t Route::getCgiLength(void)
{
	return _cgiExtensions.size();
}

bool Route::IsListing(void)
{
    return _directoryListing;
}

std::string Route::getdefaultFileForDirectory(void) const
{
    return _defaultFileForDirectory;
}

Route* Route::getRoute(unsigned int i)
{
	return &_routes[i];
}

unsigned int Route::getRoutesNumber(void) const
{
	return _routes.size();
}

void Route::setDefaultFileForDirectory(std::string const &str)
{
    _defaultFileForDirectory = str;
}

void Route::setRedirection(std::string const &str)
{
    _redirection = str;
}
void Route::setPath(std::string const &str)
{
    _path = str;
}

void Route::setDirectoryListing(bool directoryListing)
{
    _directoryListing = directoryListing;
}

void Route::setdefaultFileForDirectory(std::string const &str)
{
    _defaultFileForDirectory = str;
}

void Route::addAutorizedMethod(std::string const &str)
{
    _autorizedMethods.push_back(str);
}

void Route::setDefaultAutorizedMethod(void)
{
    _autorizedMethods.push_back("GET");
    _autorizedMethods.push_back("POST");
}

int	Route::find_len(std::string const& content, std::string::iterator const& name, char endc, bool split)
{
	int	len = 0;

	while ((name + len) != content.end() && *(name + len) != endc)
	{
		if (*(name + len) == ' ')
		{
			int	space = 0;
			while (*(name + len + space) == ' ' && (name + len + space) != content.end())
				space++;
			if (*(name + len + space) != endc)
				len += space;
			else
				return (len);
			if (name + len == content.end())
				return (-1);
			if (!split && *(name + len) != endc && *(name + len) != ' ')
				return (-2);
		}
		len++;
	}
	if ((name + len) == content.end())
		return (-1);
	return (len);
}

std::string::iterator	Route::find_end(std::string const& content, std::string::iterator i)
{
	int nb = 1;
	while (i != content.end())
	{
		if (*i == '{')
			nb++;
		if (*i == '}')
			nb--;
		if (nb == 0)
			break;
		i++;
	}
	if (nb > 0)
		Print::print(CRASH, "Parsing: } not found");
	return (i);
}

void Route::path(std::string const& content, std::string::iterator& start)
{
	int len = find_len(content, start, ';', false);
	struct stat	path_stat;
	std::string path;

	if (len == 0)
		Print::print(CRASH, "Parsing location: root is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location: root does not take multiple values");
	path = content.substr(start - content.begin(), len);
	stat(path.c_str(), &path_stat);
	if (!S_ISDIR(path_stat.st_mode))
		Print::print(CRASH, "Parsing location: " + path + " is not a directory");
	this->_path = path;
	start += len;
}

void Route::route(std::string const& content, std::string::iterator& start, std::string::iterator& end)
{
	int len = find_len(content, start, '{', false);

	if (len == 0)
		Print::print(CRASH, "Parsing location: location is missing values");
	if (len == -2)
		Print::print(CRASH, "Error: location does not take multiple parameters");
	Route route;
	route.parse(content, start, end, len);
	this->_routes.push_back(route);
}

void Route::allowmethods(std::string const& content, std::string::iterator& start)
{
	int len = find_len(content, start, ';', true);
	std::string::iterator tmp = start;
	std::string::iterator it;
	std::string method;

	if (len == 0)
		Print::print(CRASH, "Parsing location: allow_methods is missing values");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	while (start != tmp + len)
	{
		it = std::find(start, tmp + len, ' ');
		method = content.substr(start - content.begin(), it - start);
		if (method == "PUT" || method == "POST" || method == "GET")
			this->_autorizedMethods.push_back(method);
		else
			Print::print(CRASH, "Parsing location: " + method + " is not a valid method (valid methods: POST, GET, PUT)");
		start = it;
		while (*start == ' ')
			start++;
	}
}

void Route::directorylisting(std::string const& content, std::string::iterator& start)
{
	int len = find_len(content, start, ';', false);
	std::string tf;

	if (len == 0)
		Print::print(CRASH, "Parsing location: directory_listing is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location: directory_listing does not take multiple values");
	tf = content.substr(start - content.begin(), len);
	if (tf == "true")
		this->_directoryListing = true;
	else if (tf == "false")
		this->_directoryListing = false;
	else
		Print::print(CRASH, "Parsing location: " + tf + " is not a bool");
	start += len;
}

void Route::defaultfile(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string def;

	if (len == 0)
		Print::print(CRASH, "Parsing location: default_file is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location: default_file does not take multiple values");
	def = content.substr(start - content.begin(), len);
	start += len;
	if (access(def.c_str(), F_OK | R_OK))
		Print::print(CRASH, "Parsing location: " + def + " does not exist or is not executable");
	this->_defaultFileForDirectory = def;
}

void Route::cgipath(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string path;
	//check if executable
	if (len == 0)
		Print::print(CRASH, "Parsing location: cgi_path is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location: cgi_path does not take multiple values");
	
	path = content.substr(start - content.begin(), len);
	if (access(path.c_str(), F_OK | X_OK))
		Print::print(CRASH, "Parsing location: " + path + " does not exist or is not executable");
	_cgiPath = path;
	start += len;
}

void Route::cgiextension(std::string const& content, std::string::iterator& start)
{
	//.php
	int len = Route::find_len(content, start, ';', true);
	std::string::iterator tmp = start;
	std::string::iterator it;
	std::string extension;

	if (len == 0)
		Print::print(CRASH, "Parsing location: cgi_extension is missing values");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';");
	while (start != tmp + len)
	{
		it = std::find(start, tmp + len, ' ');
		extension = content.substr(start - content.begin(), it - start);
		if (extension.size() == 0 || extension[0] != '.')
			Print::print(CRASH, "Parsing location: " + extension + " is not a valid extension");
		_cgiExtensions.push_back(extension);
		start = it;
		while (*start == ' ')
			start++;
	}
}

void Route::check_name(void)
{
	if (_redirection.find('\\') != std::string::npos)
		Print::print(CRASH, "Parsing location: location can not have '\\' in the name");
	if (_redirection.find('/') != _redirection.rfind('/'))
		Print::print(CRASH, "Parsing location: location does not take multiple '/'");
}

void Route::check_duplicates(void)
{
	for (size_t i = 0; i < _routes.size(); i++)
	{
		for (size_t j = i + 1; j < _routes.size(); j++)
			if (_routes[j]._redirection == _routes[i]._redirection)
				Print::print(CRASH, "Parsing location: location can not have duplicate locations (" + _routes[j]._redirection + ")");
	}
}

void Route::parse(std::string const& content, std::string::iterator& start, std::string::iterator& end, int len)
{
	std::string::iterator it;
	std::string::iterator root_end;
	std::string param;
	bool methods = false;

	this->_redirection = content.substr(start - content.begin(), len);
	check_name();
	start += len;
	while (*(++start) == ' ')
		continue ;
	if (*start == '{')
		start++;
	root_end = find_end(content, start);
	while (*(++start) == ' ')
		continue ;
	while (start != end && start != root_end)
	{
		it = start;
		while (*start != ' ' && *start != ';' && start != end && start != root_end)
			start++;
		if (start == end || start == root_end)
			return ;
		param = content.substr(it - content.begin(), start - it);
		while (*start == ' ')
			start++;
		if (param == "root")
			path(content, start);
		else if (param == "location")
			route(content, start, root_end);
		else if (param == "allow_methods")
		{
			allowmethods(content, start);
			methods = true;
		}
		else if (param == "directory_listing")
			directorylisting(content, start);
		else if (param == "default_file")
			defaultfile(content, start);
		else if (param == "cgi_path")
			cgipath(content, start);
		else if (param == "cgi_extension")
			cgiextension(content, start);
		else
			Print::print(CRASH, "Parsing location: " + param + " is unknown");
		while ((*start == ' ' || *start == ';' || *start == '}') && start != root_end)
			start++;
	}
	start++;
	if (!methods)
		setDefaultAutorizedMethod();
	check_duplicates();
	for (size_t i = 0; i < _routes.size(); i++)
		std::cout << "route " << _redirection << " (" << i << ") : " << _routes[i]._redirection << "\n";
}