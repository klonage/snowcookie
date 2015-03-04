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
};

class DataParser
{
	static constexpr int max_bytes = 128;

	const unsigned char end_character;
	const unsigned char substitute_character;
	const unsigned char xor_character;

	int current_ptr = 0;
	unsigned char data [max_bytes];

	std::function<void(DataBuffer)> on_frame_parsed;

	bool check_crc () const;

	void parse_frame ();

public:
	virtual ~DataParser () {}
	DataParser (unsigned char end, unsigned char substitute, unsigned char xor_c);

	void register_handler (std::function<void(DataBuffer)> func);

	void append_bytes (char buff[], int size);
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_DATA_PARSER_H_ */
