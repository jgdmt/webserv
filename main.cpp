/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:40:09 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 11:53:59 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "parsing/Settings.hpp"

int main(int argc, char **argv)
{
    Settings settings;
    if (argc == 1)
        settings.parse("default/webserv.conf");
    else if (argc > 2)
    {
        std::cerr << "Wrong Numbers of param\n";
        return 1;
    }
    else
        settings.parse(argv[1]);
    
}