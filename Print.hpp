/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:47 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/13 18:25:47 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>


#define DEBUG "[DEBUG]"
#define INFO "[INFO]"
#define ERROR "[ERROR]"
#define CRASH "[CRASH]"
#define SUCCESS "[SUCCESS]"
class Server;

class Print
{
    public:
        static void print(std::string const &status, Server &serv, std::string const &str);
        static void print(std::string const &status, std::string const &str);
        static void error_print(std::string const &status, std::string const &str);
    private:

        Print();
        ~Print();
};