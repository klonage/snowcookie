/*
 * data_parser.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#include "data_parser.h"
#include "edison-board/logger.h"

#include "protocol_utils.h"

using namespace SnowCookie;

void DataParser::register_handler (std::function<void(DataBuffer)> func)
{
	on_frame_parsed = func;
}

void DataParser::append_bytes (const char* buff, int size)
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
	unsigned char new_data [max_bytes];

	int size = unpack_frame (data, current_ptr - 1, new_data, substitute_character, xor_character);

	if (!check_crc (new_data, size-1, new_data[size - 1]))
	{
		Logger::log ("invalid crc");
		return;
	}

	DataBuffer buf = {data, current_ptr, new_data, size};
	on_frame_parsed (buf);
}

