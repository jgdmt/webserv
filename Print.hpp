/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:20:47 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 17:01:45 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>

class Print
{
    public:
        static void print(std::string const &str);
        static void success_print(std::string const &str);
        static void error_print(std::string const &str);
    private:
        Print();
        ~Print();
};