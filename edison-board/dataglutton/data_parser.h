/*
 * data_parser.h
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_DATA_PARSER_H_
#define EDISON_BOARD_DATAGLUTTON_DATA_PARSER_H_

#include <functional>

namespace SnowCookie {

struct DataBuffer
{
	unsigned char *data;
	long long timestamp;
	int size;
	unsigned char *frame;
	int frame_size;
};

class DataParser
{
	static constexpr int max_bytes = 128;

	const unsigned char end_character = 0x0A;
	const unsigned char substitute_character = 0x1A;
	const unsigned char xor_character = 0x33;

	int current_ptr = 0;
	unsigned char data [max_bytes];

	std::function<void(DataBuffer)> on_frame_parsed;

	bool check_crc (unsigned char *data, int new_size) const;

	void parse_frame ();

	int unpack_frame (unsigned char *data);

public:
	virtual ~DataParser () {}

	void register_handler (std::function<void(DataBuffer)> func);

	void append_bytes (char buff[], int size);
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_DATA_PARSER_H_ */
