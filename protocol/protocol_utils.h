/*
 * protocol_utils.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef PROTOCOL_UTILS_H_
#define PROTOCOL_UTILS_H_

namespace SnowCookie {

bool check_crc (unsigned char *data, int size, char expected_crc);

int unpack_frame (unsigned char *frame, int size, char substitute_char, char xor_char);

// this method adds crc and end frame character, returns new data size
int pack_frame (unsigned char *data, int size, unsigned char* destination, char substitute_char, char xor_char, char end_char);

}

#endif /* PROTOCOL_UTILS_H_ */
