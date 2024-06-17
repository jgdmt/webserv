/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:58:44 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/17 15:32:15 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Print.hpp"

class Request
{
    public:
        void add(std::string const &new_buff);
        bool parse(void);
    private:
      std::string _buffer;
	  std::string _host;
	  std::string _connection;
	  std::string _method;
	  std::string _uri;
	  
};