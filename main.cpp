/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:40:09 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 16:45:34 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Print.hpp"
#include <iostream>
#include "parsing/Settings.hpp"

int main(int argc, char **argv)
{
    Settings settings;

    switch(argc)
    {
        case 1:
            settings.parse("default/webserv.conf");
            break;
        case 2:
          settings.parse(argv[1]);
            break;
        default:
            std::cerr << "Wrong Numbers of param\n";
            return 1;
    }
    
}