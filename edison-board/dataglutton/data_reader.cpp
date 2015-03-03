#include "data_reader.h"
#include "logger.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <sys/stat.h>

using namespace SnowCookie;

DataReader::DataReader(const std::string& filename)
 : filename(filename)
{
}

void DataReader::init()
{
	file.open(filename);

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);
}

void DataReader::start()
{
	std::lock_guard<std::mutex> lock (start_stop);
	run = true;
	while (run)
	{
		file << "hej" << std::endl;
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
	close();
}

void DataReader::close()
{
	Logger::log ("closing file ", filename);
	file.close();
}

void DataReader::stop()
{
	run = false;
	std::lock_guard<std::mutex> lock (start_stop);
}

std::string DataReader::generate_unique_filename(std::string name)
{
	struct stat buffer;
	std::string new_name = name;
	int i = 0;
	while (stat (new_name.c_str(), &buffer) == 0)
	{
		new_name = name + "(" + std::to_string(i++) + ")";
	}

	return new_name;
}

