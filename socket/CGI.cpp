/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:00:16 by jgoudema          #+#    #+#             */
/*   Updated: 2024/06/20 15:37:22 by jgoudema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI()
{
	
}

void	CGI::createEnv(void)
{
	_env["SERVER_SOFTWARE"] = "?";
	_env["SERVER_NAME"] = "?";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = "?";
	_env["REQUEST_METHOD"] = "?";
	_env["PATH_INFO"] = "?";
	_env["PATH_TRANSLATED"] = "?";
	_env["SCRIPT_NAME"] = "?";
	_env["QUERY_STRING"] = "?";
	_env["REMOTE_HOST"] = "?";
	_env["REMOTE_ADDR"] = "?";
	_env["AUTH_TYPE"] = "?";
	_env["REMOTE_USER"] = "?";
	_env["REMOTE_IDENT"] = "?";
	_env["CONTENT_TYPE"] = "?";
	_env["CONTENT_LENGTH"] = "?";
}