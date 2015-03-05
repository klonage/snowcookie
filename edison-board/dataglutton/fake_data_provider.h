/*
 * fake_data_provider.h
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_FAKE_DATA_PROVIDER_H_
#define EDISON_BOARD_FAKE_DATA_PROVIDER_H_

#include "iplug_device.h"

namespace SnowCookie {

class FakeDataProvider : public IPlugDevice
{
	static int get_crc(const char buffer[], int size);
	int counter = 0;
	unsigned long long total = 0;

public:
	int read_data (char buffer[], int max_size) override;
	int write_data (unsigned char buffer[], int size) override;
	void init (const std::string& source) override;
	void close () override;
};

}

#endif /* EDISON_BOARD_FAKE_DATA_PROVIDER_H_ */
