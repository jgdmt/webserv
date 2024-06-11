/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:47 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 17:59:59 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>

#define DEBUG "[DEBUG]"
#define INFO "[INFO]"
#define ERROR "[ERROR]"
#define CRASH "[CRASH]"

class Print
{
    public:
        static void print(std::string status, std::string const &str);
        static void success_print(std::string status, std::string const &str);
        static void error_print(std::string status, std::string const &str);
    private:
        Print();
        ~Print();
};