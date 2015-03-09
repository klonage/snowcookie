/*
 * protocol_utils.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef PROTOCOL_UTILS_H_
#define PROTOCOL_UTILS_H_

namespace SnowCookie {

const char end_character = 0x0A;
const char substitute_character = 0x1A;
const char xor_character = 0x33;

bool check_crc (char *data, int size, char expected_crc);

int unpack_frame (char *data, int size, char* destination);

// this method adds crc and end frame character, returns new data size
int pack_frame (char *data, int size, char* destination);

}

#endif /* PROTOCOL_UTILS_H_ */
