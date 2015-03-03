#ifndef _SNOWCOOKIE_LOGGER_H
#define _SNOWCOOKIE_LOGGER_H

#include <string>
#include <iostream>

namespace SnowCookie {

class Logger
{
public:
	virtual ~Logger () {}
	template<typename T>
	static void log(T&& arg) { std::cout << arg << std::endl; }

	template<typename First, typename... Rest>
	static void log(First&& first, Rest&& ...rest)
	{
		std::cout << first;
		log(rest...);
	}
};

}
#endif
