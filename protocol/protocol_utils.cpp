/*
 * protocol_utils.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "protocol_utils.h"

namespace SnowCookie {

static char calculate_crc (unsigned char * data, int size)
{
	char crc = 0;
	for (int i = 0; i < size; i++)
		crc ^= data [i];

	return crc;
}

bool check_crc (unsigned char *data, int size, char expected_crc)
{
	char crc = calculate_crc (data, size);

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

int pack_frame (unsigned char *data, int size, unsigned char* destination, char substitute_char, char xor_char, char end_char)
{
	int new_size = 0;
	char crc = calculate_crc (data, size);

	for (int i = 0; i < size; i++)
	{
		if (data[i] != end_char && data[i] != substitute_char)
			destination[new_size++] = data[i];
		else
		{
			destination [new_size++] = substitute_char;
			destination [new_size++] = data [i] ^ xor_char;
		}
	}

	// todo refactor me!
	if (crc != end_char && crc != substitute_char)
		destination[new_size++] = crc;
	else
	{
		destination [new_size++] = substitute_char;
		destination [new_size++] = crc ^ xor_char;
	}

	destination[new_size++] = end_char;

	return new_size;
}

}
