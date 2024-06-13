/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:00:33 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 18:12:48 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include "../Print.hpp"
#include <iterator>

typedef std::string::iterator it;

static std::string createContent(std::string const& fileName)
{
	std::ifstream ifs(fileName);
	std::string	line;
	std::string content;

	if (!ifs || !ifs.is_open())
	{
		Print::error_print(ERROR, "File error.");
		ifs.open("default/webserv.conf");
	}
	while (ifs.good())
	{
		std::getline(ifs, line);
		content += line;
	}
	ifs.close();
	std::replace(content.begin(), content.end(), '\t', ' ');
	return (content);
}

static it	find_end(std::string const& content, it i)
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
		Print::error_print(CRASH, "Bracket error: } not found");
	return (i);
}

void Settings::parseServer(std::string const& content, it& name, it& start, it& end)
{
	int len = Route::find_len(content, name, '{', true);
	
	if (name == start || content.substr(name - content.begin(), len) != "server")
		Print::error_print(CRASH, content.substr(name - content.begin(), len) + " is an unknown key");
	Server server(this->_servers.size() + 1);
	server.parse(content, start, end);
	this->_servers.push_back(server);
}

void Settings::parse(std::string const &fileName)
{
	std::string content = createContent(fileName);
	it	start = content.begin();
	it	name;

	if (start == content.end())
		Print::error_print(CRASH, "Parsing: file is empty");
	while (start != content.end())
	{
		name = start;
		while (start != content.end() && *start != '{')
		{
			if (*start == '}')
				Print::error_print(CRASH, "Bracket error: } has no {");
			if (*name == ' ')
				name++;
			start++;
		}
		it end = find_end(content, start + 1);
		if (end == content.end())
			return ;
		parseServer(content, name, start, end);
		start++;
	}
}