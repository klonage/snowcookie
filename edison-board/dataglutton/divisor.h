/*
 * divisor.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_DIVISOR_H_
#define EDISON_BOARD_DATAGLUTTON_DIVISOR_H_

namespace SnowCookie {

class Divisor
{
	int divisions [3] = {0};
	int divs_count [3] = {0};
public:
	Divisor (int p1, int p2, int p3)
	{
		divisions [0] = p1; divisions [1] = p2; divisions [2] = p3;
	}
	void set_division (int number, int value)
	{
		if (number > 2)
			return;

		divisions [number] = value;
	}

	void update (int number)
	{
		if (number > 2)
			return;

		divs_count [number] ++;
	}

	bool can_push (int number)
	{
		if (number > 2)
			return true;

		return divisions [number] != 0 && divs_count [number] % divisions [number] == 0;
	}
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_DIVISOR_H_ */
