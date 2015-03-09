/*
 * protocol_utils.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "protocol_utils.h"

namespace SnowCookie {

bool check_crc (unsigned char *data, int size, char expected_crc)
{
	char crc = 0;
	for (int i = 0; i < size; i++)
		crc ^= data [i];

	return crc == expected_crc;
}

int unpack_frame (unsigned char *data, int size, char substitute_char, char xor_char)
{
	int new_size = 0;

	for (int i = 0; i < size; i++)
	{
		if (data [i] == substitute_char)
		{
			data [new_size++] = (char)(data [i + 1] ^ xor_char);
			i++;
		}
		else
			data [new_size++] = data [i];
	}

	return new_size;
}

}
