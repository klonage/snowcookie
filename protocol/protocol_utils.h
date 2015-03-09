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

int unpack_frame (unsigned char *data, int size, char substitute_char, char xor_char);

}

#endif /* PROTOCOL_UTILS_H_ */
