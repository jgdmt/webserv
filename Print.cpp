/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:53 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 18:30:13 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Print.hpp"
#include "parsing/Server.hpp"

static void output_time(void)
{
    std::time_t now = std::time(nullptr);
    std::tm *time =  std::localtime(&now);
    std::cout << "[" << time->tm_year + 1900 << '-';
    if (time->tm_mon + 1 < 10)
        std::cout << '0';
    std::cout << time->tm_mon + 1 << '-';
    if (time->tm_mday < 10)
        std::cout << '0';
    std::cout << time->tm_mday << "][";
    if (time->tm_hour < 10)
        std::cout << '0';
    std::cout << time->tm_hour << ':';
    if (time->tm_min < 10)
        std::cout << '0';   
    std::cout << time->tm_min << ':';
    if (time->tm_sec < 10)
        std::cout << '0';   
    std::cout << time->tm_sec << "]     ";
}

static int getLevel(std::string const &level)
{
	std::string list[] = {
		DEBUG,
		INFO,
		ERROR,
		CRASH,
	};
	for(int i = 0; i < 4; i++)
	{
		if (list[i] == level)
			return (i);
	}
	return(-1);
}

void Print::print(std::string const &status, Server &serv, std::string const &str)
{
    int level = getLevel(status);
    if(level >= getLevel(serv.getLogLevel()))
    {
        output_time();
        std::cout <<  status << "[" << serv.getID() << "]    " << str <<std::endl;
    }
}

void Print::print(std::string const &status, std::string const &str)
{
    if(status == SUCCESS)
    {
        std::cout << "\e[1;32m";
        output_time();
        std::cout << str << std::endl;
        std::cout << "\e[97m";
    }
    else if(status == CRASH || status == ERROR)
        error_print(status, str);

}

void Print::error_print(std::string const &status, std::string const &str)
{
    std::time_t now = std::time(nullptr);
    std::tm *time =  std::localtime(&now);
    std::cerr << "\e[31m[" << time->tm_year + 1900 << '-';
    if (time->tm_mon + 1 < 10)
        std::cerr << '0';
    std::cerr << time->tm_mon + 1 << '-';
    if (time->tm_mday < 10)
        std::cerr << '0';
    std::cerr << time->tm_mday << "][";
    if (time->tm_hour < 10)
        std::cerr << '0';
    std::cerr << time->tm_hour << ':';
    if (time->tm_min < 10)
        std::cerr << '0';   
    std::cerr << time->tm_min << ':';
    if (time->tm_sec < 10)
        std::cerr << '0';   
    std::cerr << time->tm_sec << "]    " << status << "    " << str << "\e[97m\n";
    if(status == CRASH)
        exit(EXIT_FAILURE);
}