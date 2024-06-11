/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:53 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 17:02:48 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Print.hpp"

static std::tm *getTime(void)
{
    std::time_t now = std::time(nullptr);
    return std::localtime(&now);

}

void Print::print(std::string const &str)
{
    std::tm *time = getTime();
    std::cout << '[' << time->tm_year + 1900 << '-' << time->tm_mon + 1 << '-' << time->tm_mday << "][" << time->tm_hour << ':' << time->tm_min << ':' << time->tm_sec << "]    " << str << '\n';
}

void Print::success_print(std::string const &str)
{
    std::tm *time = getTime();
    std::cout << "\e[32m[" << time->tm_year + 1900 << '-' << time->tm_mon + 1 << '-' << time->tm_mday << "][" << time->tm_hour << ':' << time->tm_min << ':' << time->tm_sec << "]    " << str << "\e[30m\n";
}

void Print::error_print(std::string const &str)
{
    std::tm *time = getTime();
    std::cerr << "\e[31m[" << time->tm_year + 1900 << '-' << time->tm_mon + 1 << '-' << time->tm_mday << "][" << time->tm_hour << ':' << time->tm_min << ':' << time->tm_sec << "]    " << str << "\e[30m\n";

}