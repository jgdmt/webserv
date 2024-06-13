/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:00:33 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 10:08:24 by vilibert         ###   ########.fr       */
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

static it	find_end(std::string content, it i)
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
		Print::error_print(ERROR, "Bracket error: } not found");
	return (i);
}

static int	find_len(std::string const& content, it const& name, char endc, bool split)
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
	return (len);
}

void Settings::parseServer(std::string const& content, it& name, it& start, it& end)
{
	int len = find_len(content, name, '{', true);
	
	if (name == start || content.substr(name - content.begin(), len) != "server")
	{
		Print::error_print(ERROR, "server");
		exit (1);
	}
	Server server(this->_servers.size() + 1);
	server.parse(content, start, end);
	this->_servers.push_back(server);
}

void Settings::parse(std::string const &fileName)
{
	std::string content = createContent(fileName);
	it	start = content.begin();
	it	name = content.begin();


	while (start != content.end())
	{
		while (start != content.end() && *start != '{')
		{
			if (*start == '}')
				Print::error_print(ERROR, "Bracket error: } has no {");
			if (*name == ' ')
				name++;
			start++;
		}
		// std::cout << std::distance(name, content.begin()) << "\n";
		it end = find_end(content, start + 1);
		if (end == content.end())
			return ;
		parseServer(content, name, start, end);
		break;
	}
}