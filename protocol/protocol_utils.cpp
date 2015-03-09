/*
 * protocol_utils.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "protocol_utils.h"

namespace SnowCookie {

static char calculate_crc (char * data, int size)
{
	char crc = 0;
	for (int i = 0; i < size; i++)
		crc ^= data [i];

	return crc;
}

bool check_crc (char *data, int size, char expected_crc)
{
	char crc = calculate_crc (data, size);

	return crc == expected_crc;
}

int unpack_frame (char *data, int size, char* destination)
{
	int new_size = 0;

	for (int i = 0; i < size; i++)
	{
		if (data [i] == substitute_character)
		{
			destination [new_size++] = (char)(data [i + 1] ^ xor_character);
			i++;
		}
		else
			destination [new_size++] = data [i];
	}

	return new_size;
}

int pack_frame (char *data, int size, char* destination)
{
	int new_size = 0;
	char crc = calculate_crc (data, size);

	for (int i = 0; i < size; i++)
	{
		if (data[i] != end_character && data[i] != substitute_character)
			destination[new_size++] = data[i];
		else
		{
			destination [new_size++] = substitute_character;
			destination [new_size++] = data [i] ^ xor_character;
		}
	}

	// todo refactor me!
	if (crc != end_character && crc != substitute_character)
		destination[new_size++] = crc;
	else
	{
		destination [new_size++] = substitute_character;
		destination [new_size++] = crc ^ xor_character;
	}

	destination[new_size++] = end_character;

	return new_size;
}

}
