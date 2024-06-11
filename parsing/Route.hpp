/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:13:40 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/11 16:19:22 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <algorithm>
#include <string>

class Route
{
    public:
        Route();
        bool _isAutorizedMethod(std::string const &method);
        std::string const &getRedirection(void) const;
        std::string const &getPath(void) const;
        bool IsListing(void);
        std::string getdefaultFileForDirectory(void) const;
        void setDefaultFileForDirectory(std::string const &str);
        void setRedirection(std::string const &str);
        void setPath(std::string const &str);
        void setDirectoryListing(bool directoryListing);
        void setdefaultFileForDirectory(std::string const &str);
        void addAutorizedMethod(std::string const &str);
        void setDefaultAutorizedMethod(void);
    private:
        Route(Route const &cpy);
        Route &operator= (Route const &cpy);
        std::vector<std::string> _autorizedMethods;
        std::string _redirection;
        std::string _path;
        bool _directoryListing;
        std::string _defaultFileForDirectory;
        

};