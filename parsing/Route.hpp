/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoudema <jgoudema@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:13:40 by vilibert          #+#    #+#             */
/*   Updated: 2024/07/10 20:13:26 by jgoudema         ###   ########.fr       */
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
        bool isAutorizedMethod(std::string const &method);
        std::string const &getRedirection(void) const;
        std::string const &getLocation(void) const;
        std::string const &getRoot(void) const;
        bool IsListing(void);
        std::string getdefaultFileForDirectory(void) const;
		std::string const &getCgiPath(void) const;
		std::string const &getCgiExtension(int i) const;
		std::string const &getUploadDirectory(void) const;
		size_t getCgiLength(void);
        Route *getRoute(unsigned int i);
        unsigned int getRoutesNumber(void) const;
        void setDefaultFileForDirectory(std::string const &str);
        void setLocation(std::string const &str);
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
        std::string _location;
        std::string _root;
		std::string _uploadDirectory;
        bool _directoryListing;
        std::string _defaultFileForDirectory;
		std::string _cgiPath;
        std::string _redirection;
		std::vector<std::string> _cgiExtensions;
        
		void	root(std::string const& content, std::string::iterator& start);
		void	route(std::string const& content, std::string::iterator& start, std::string::iterator& end);
		void	allowmethods(std::string const& content, std::string::iterator& start);
		void	directorylisting(std::string const& content, std::string::iterator& start);
		void	defaultfile(std::string const& content, std::string::iterator& start);
		void	cgipath(std::string const& content, std::string::iterator& start);
		void	cgiextension(std::string const& content, std::string::iterator& start);
		void	upload(std::string const& content, std::string::iterator& start);
        void    redirect(std::string const& content, std::string::iterator&start);
		void	check_name(void);
		void	check_duplicates(void);
		void	check_defaultfile(void);
};