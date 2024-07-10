/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:32:07 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 20:13:21 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route()
{
    _directoryListing = true;
}

bool Route::isAutorizedMethod(std::string const &method)
{
    if(find(_autorizedMethods.begin(), _autorizedMethods.end(), method) != _autorizedMethods.end())
        return true;
    else
        return false;
}

std::string const &Route::getLocation(void) const
{
    return _location;
}

std::string const &Route::getRedirection(void) const
{
    return _redirection;
}

std::string const &Route::getRoot(void) const
{
    return _root;
}

std::string const &Route::getCgiPath(void) const
{
	return _cgiPath;
}

std::string const &Route::getCgiExtension(int i) const
{
	return _cgiExtensions[i];
}

std::string const& Route::getUploadDirectory(void) const
{
	return _uploadDirectory;
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

void Route::setLocation(std::string const &str)
{
    _location = str;
}
void Route::setRedirection(std::string const &str)
{
    _redirection = str;
}
void Route::setPath(std::string const &str)
{
    _root = str;
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

void Route::root(std::string const& content, std::string::iterator& start)
{
	int len = find_len(content, start, ';', false);
	struct stat	path_stat;
	std::string path;

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": root is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location: missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": root does not take multiple values");
	path = content.substr(start - content.begin(), len);
	if (path[path.length() - 1] == '/')
		path.pop_back();
	if (stat(path.c_str(), &path_stat) == -1 || !S_ISDIR(path_stat.st_mode))
		Print::print(CRASH, "Parsing location " + _location + ": " + path + " is not a directory");
	this->_root = path;
	start += len;
}

void Route::route(std::string const& content, std::string::iterator& start, std::string::iterator& end)
{
	int len = find_len(content, start, '{', false);

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": location is missing values");
	if (len == -2)
		Print::print(CRASH, "Parsing location  " + _location + ": location does not take multiple parameters");
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
		Print::print(CRASH, "Parsing location " + _location + ": allow_methods is missing values");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	while (start != tmp + len)
	{
		it = std::find(start, tmp + len, ' ');
		method = content.substr(start - content.begin(), it - start);
		if (method == "PUT" || method == "POST" || method == "GET" || method == "DELETE")
			this->_autorizedMethods.push_back(method);
		else
			Print::print(CRASH, "Parsing location " + _location + ": " + method + " is not a valid method (valid methods: POST, GET, PUT, DELETE)");
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
		Print::print(CRASH, "Parsing location " + _location + ": directory_listing is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": directory_listing does not take multiple values");
	tf = content.substr(start - content.begin(), len);
	if (tf == "true")
		this->_directoryListing = true;
	else if (tf == "false")
		this->_directoryListing = false;
	else
		Print::print(CRASH, "Parsing location " + _location + ": " + tf + " is not a bool");
	start += len;
}

void Route::defaultfile(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string def;

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": default_file is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": default_file does not take multiple values");
	def = content.substr(start - content.begin(), len);
	start += len;	
	this->_defaultFileForDirectory = def;
}

void Route::cgipath(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string path;

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": cgi_path is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": cgi_path does not take multiple values");
	path = content.substr(start - content.begin(), len);
	if (access(path.c_str(), F_OK | X_OK))
		Print::print(CRASH, "Parsing location " + _location + ": " + path + " does not exist or is not executable");
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
		Print::print(CRASH, "Parsing location " + _location + ": cgi_extension is missing values");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';");
	while (start != tmp + len)
	{
		it = std::find(start, tmp + len, ' ');
		extension = content.substr(start - content.begin(), it - start);
		if (extension.size() == 0 || extension[0] != '.')
			Print::print(CRASH, "Parsing location " + _location + ": " + extension + " is not a valid extension");
		_cgiExtensions.push_back(extension);
		start = it;
		while (*start == ' ')
			start++;
	}
}

void Route::upload(std::string const& content, std::string::iterator& start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string path;
	struct stat path_stat;

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": upload is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": upload does not take multiple values");
	path = content.substr(start - content.begin(), len);
	if (stat(path.c_str(), &path_stat) == -1 || !S_ISDIR(path_stat.st_mode))
		Print::print(CRASH, "Parsing location (upload) " + _location + ": " + path + " is not a directory");
	if (path[0] != '/')
		Print::print(CRASH, "Parsing location " + _location + ": upload takes only absolute paths");
	this->_uploadDirectory = path;
	start += len;
}

void Route::redirect(std::string const& content, std::string::iterator&start)
{
	int len = Route::find_len(content, start, ';', false);
	std::string path;

	if (len == 0)
		Print::print(CRASH, "Parsing location " + _location + ": redirect is missing a value");
	if (len == -1)
		Print::print(CRASH, "Parsing location " + _location + ": missing ';'");
	if (len == -2)
		Print::print(CRASH, "Parsing location " + _location + ": redirect does not take multiple values");
	path = content.substr(start - content.begin(), len);
	_redirection = path;
	if (_redirection[_redirection.length() - 1] != '/')
		_redirection.append("/");
	start += len;
}

void Route::check_name(void)
{
	if (_location.find('\\') != std::string::npos)
		Print::print(CRASH, "Parsing location " + _location + ": location can not have '\\' in the name");
	if (_location.find('/') != _location.rfind('/'))
		Print::print(CRASH, "Parsing location " + _location + ": location does not take multiple '/'");
	if (_location[0] != '/')
		Print::print(ERROR, "Warning: unreachable location: " + _location);
}

void Route::check_duplicates(void)
{
	for (size_t i = 0; i < _routes.size(); i++)
	{
		for (size_t j = i + 1; j < _routes.size(); j++)
			if (_routes[j]._location == _routes[i]._location)
				Print::print(CRASH, "Parsing location " + _location + ": location can not have duplicate locations (" + _routes[j]._location + ")");
	}
}

void Route::check_defaultfile(void)
{
	std::string def = _root;
	if (def[def.size() - 1] != '/')
		def.append("/");
	def.append(_defaultFileForDirectory);
	if (access(def.c_str(), F_OK | R_OK))
	{
		Print::print(ERROR, "Parsing location " + _location + ": " + def + " does not exist or is not executable");
		_defaultFileForDirectory.clear();
	}
}

void Route::parse(std::string const& content, std::string::iterator& start, std::string::iterator& end, int len)
{
	std::string::iterator it;
	std::string::iterator root_end;
	std::string param;
	bool methods = false;

	this->_location = content.substr(start - content.begin(), len);
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
			root(content, start);
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
		else if (param == "upload")
			upload(content, start);
		else if (param == "redirect")
			redirect(content, start);
		else
			Print::print(CRASH, "Parsing location: " + param + " is unknown");
		while ((*start == ' ' || *start == ';' || *start == '}') && start != root_end)
			start++;
	}
	start++;
	if (_root.length() == 0 && _redirection.length() == 0)
		Print::print(CRASH, "Parsing location " + _location + ": missing _root");
	if (!methods)
		setDefaultAutorizedMethod();
	if (_defaultFileForDirectory.size() > 0)
		check_defaultfile();
	check_duplicates();
}