/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:53 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 19:01:54 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Print.hpp"

static std::tm *getTime(void)
{
    std::time_t now = std::time(nullptr);
    return std::localtime(&now);

}

void Print::print(std::string status, std::string const &str)
{
    std::tm *time = getTime();
    std::cout << '[' << time->tm_year + 1900 << '-';
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
    std::cout << time->tm_sec << "]    " << status << "    " << str << '\n';
}

void Print::success_print(std::string status, std::string const &str)
{
    std::cout << "\e[32m";
    print(status, str);
    std::cout << "\e[97m";
}

void Print::error_print(std::string status, std::string const &str)
{
    std::tm *time = getTime();
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

}