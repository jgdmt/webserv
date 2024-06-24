/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:13:40 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/24 14:31:05 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Print.hpp"

class Route
{
    public:
        Route();
        bool _isAutorizedMethod(std::string const &method);
        std::string const &getRedirection(void) const;
        std::string const &getPath(void) const;
        bool IsListing(void);
        std::string getdefaultFileForDirectory(void) const;
        Route *getRoute(unsigned int i);
        unsigned int getRoutesNumber(void) const;
        void setDefaultFileForDirectory(std::string const &str);
        void setRedirection(std::string const &str);
        void setPath(std::string const &str);
        void setDirectoryListing(bool directoryListing);
        void setdefaultFileForDirectory(std::string const &str);
        void addAutorizedMethod(std::string const &str);
        void setDefaultAutorizedMethod(void);

		void parse(std::string const& content, std::string::iterator& start, std::string::iterator& end, int len);
		static int	find_len(std::string const& content, std::string::iterator const& name, char endc, bool split);
		static std::string::iterator	find_end(std::string const& content, std::string::iterator i);

	private:
        // Route(Route const &cpy);
        // Route &operator= (Route const &cpy);
        std::vector<std::string> _autorizedMethods;
		std::vector<Route> _routes;
        std::string _redirection;
        std::string _path;
        bool _directoryListing;
        std::string _defaultFileForDirectory;
		std::string _cgiPath;
		std::vector<std::string> _cgiExtensions;
        
		void	path(std::string const& content, std::string::iterator& start);
		void	route(std::string const& content, std::string::iterator& start, std::string::iterator& end);
		void	allowmethods(std::string const& content, std::string::iterator& start);
		void	directorylisting(std::string const& content, std::string::iterator& start);
		void	defaultfile(std::string const& content, std::string::iterator& start);
		void	cgipath(std::string const& content, std::string::iterator& start);
		void	cgiextension(std::string const& content, std::string::iterator& start);
		void	check_name(void);
		void	check_duplicates(void);
};