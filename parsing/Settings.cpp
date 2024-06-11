/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:00:33 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 17:02:14 by vilibert         ###   ########.fr       */
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

void Settings::parseServer(std::string& content, it& name, it& start, it& end)
{
	// if (name == start) 
	// 	Server server;
	// else	
		Server server(content.substr(name - content.begin(), start - name)); //Skip space & tab in name
	std::cout << server.getName() << "\n";
	this->_servers.push_back(server);
	(void) end;
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
			start++;
		}
		std::cout << std::distance(name, content.begin()) << "\n";
		it end = find_end(content, start + 1);
		if (end == content.end())
			return ;
		parseServer(content, name, start, end);
		break;
	}
}