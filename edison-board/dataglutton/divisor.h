/*
 * divisor.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_DIVISOR_H_
#define EDISON_BOARD_DATAGLUTTON_DIVISOR_H_

#include <map>

namespace SnowCookie {

class Divisor
{
	std::map<char, unsigned short> divisors;
	std::map<char, int> divisors_count;

	bool check_div_exists (int number)
	{
		return divisors_count.find(number) != divisors_count.end();
	}

public:
	Divisor (std::map<char, unsigned short> divisions)
	{
		divisors = divisions;
		for (auto d : divisions)
			divisors_count[d.first] = 0;
	}
	void set_division (char number, unsigned short value)
	{
		divisors [number] = value;
		if (!check_div_exists (number))
			divisors_count [number] = 0;
	}

	void update (char number)
	{
		if (check_div_exists (number))
			divisors_count [number] ++;
	}

	bool can_push (char number)
	{
		if (!check_div_exists (number))
			return true;
		return divisors [number] != 0 && divisors_count [number] % divisors [number] == 0;
	}
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_DIVISOR_H_ */
