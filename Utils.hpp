#pragma once

#include <string>
#include <sstream>

template <class T>
static T convertType(std::string entry)
{
	T s = 0;
	std::stringstream	ss(entry);

	ss >> s;
	return (s);
}