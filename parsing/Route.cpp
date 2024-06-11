/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:32:07 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 16:08:43 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route()
{
    _path = "default/web";
    _directoryListing = true;
    _defaultFileForDirectory = "default/directory.html";
}

bool Route::_isAutorizedMethod(std::string const &method)
{
    if(find(_autorizedMethods.begin(), _autorizedMethods.end(), method) != _autorizedMethods.end())
        return true;
    else
        return false;
}

std::string const &Route::getRedirection(void) const
{
    return _redirection;
}

std::string const &Route::getPath(void) const
{
    return _path;
}

bool Route::IsListing(void)
{
    return _directoryListing;
}

std::string Route::getdefaultFileForDirectory(void) const
{
    return _defaultFileForDirectory;
}

void Route::setDefaultFileForDirectory(std::string const &str)
{
    _defaultFileForDirectory = str;
}

void Route::setRedirection(std::string const &str)
{
    _redirection = str;
}
void Route::setPath(std::string const &str)
{
    _path = str;
}

void Route::setDirectoryListing(bool directoryListing)
{
    _directoryListing = directoryListing;
}

void Route::setdefaultFileForDirectory(std::string const &str)
{
    _defaultFileForDirectory = str;
}

void Route::addAutorizedMethod(std::string const &str)
{
    _autorizedMethods.push_back(str);
}

void Route::setDefaultAutorizedMethod(void)
{
    _autorizedMethods.push_back("GET");
    _autorizedMethods.push_back("POST");
}