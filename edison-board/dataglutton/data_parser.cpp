/*
 * data_parser.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#include "data_parser.h"
#include "logger.h"

#include <chrono>

using namespace SnowCookie;

DataParser::DataParser (unsigned char end, unsigned char substitute, unsigned char xor_c)
: end_character (end), substitute_character (substitute), xor_character (xor_c)
{
}

void DataParser::register_handler (std::function<void(DataBuffer)> func)
{
	on_frame_parsed = func;
}

void DataParser::append_bytes (char buff[], int size)
{
	for (int i = 0; i < size; i++)
	{
		data [current_ptr++] = buff[i];
		if (buff [i] == end_character)
		{
			parse_frame ();
			current_ptr = 0;
		}
		else if (current_ptr == max_bytes)
		{
			Logger::log ("Error: too many bytes without end character. Flushing...");
			current_ptr = 0;
		}
	}
}

void DataParser::parse_frame ()
{
	if (!check_crc ())
	{
		Logger::log ("invalid crc");
		return;
	}

	auto now = std::chrono::high_resolution_clock::now ();
	long long timestamp = std::chrono::duration_cast<std::chrono::microseconds> (now.time_since_epoch ()).count ();

	DataBuffer buf = {data, timestamp, current_ptr};
	on_frame_parsed (buf);
}

bool DataParser::check_crc () const
{
	unsigned char new_data [max_bytes];
	int new_size = 0;
	unsigned char crc = 0;

	// unpacking
	for (int i = 0; i < current_ptr - 1; i++)
	{
		if (data [i] == substitute_character)
		{
			new_data [new_size++] = (char)(data [i + 1] ^ xor_character);
			i++;
		}
		else
			new_data [new_size++] = data [i];
	}

	for (int i = 0; i < new_size-1; i++)
		crc ^= new_data [i];

	auto expected_crc = new_data [new_size-1];
	return crc == expected_crc;
}
