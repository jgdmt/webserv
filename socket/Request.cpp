/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:32 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/14 15:13:30 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::add(std::string const &new_buff)
{
    _buffer += new_buff; 
}

bool Request::parse(void)
{
    std::cout << _buffer;
    return 1;
}