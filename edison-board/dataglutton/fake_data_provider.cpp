/*
 * fake_data_provider.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#include "fake_data_provider.h"

#include "logger.h"

#include <cassert>

#include <algorithm>
#include <chrono>
#include <thread>

using namespace SnowCookie;

int FakeDataProvider::get_crc(const char buffer[], int size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum ^= buffer[i];
	return sum;
}

int FakeDataProvider::read_data (char buffer[], int max_size)
{
	std::this_thread::sleep_for (std::chrono::milliseconds(1));
	std::string t = "test";
	assert (max_size >= 6);
	std::copy(t.begin(), t.end(), buffer);

	buffer[4] = get_crc(t.c_str(), t.length());
	buffer[5] = 0x0A;
	total += 6;
	counter++;

	return 6;
}

void FakeDataProvider::init (const std::string& source)
{
	total = 0;
	counter = 0;
}

void FakeDataProvider::close ()
{
	Logger::log ("Generated: ", counter, " data packets(total size: ", total, " bytes)");
}

int FakeDataProvider::write_data (char buffer[], int size)
{
	Logger::log ("Has data in fake data provider of length", size);
	return size;
}
