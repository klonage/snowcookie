#include "data_reader.h"
#include "logger.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <sys/stat.h>

using namespace SnowCookie;

DataReader::DataReader(const std::string& filename)
 : filename(filename),
   parser(0x0A, 0x1A, 0x33)
{
}

void DataReader::init()
{
	file.open(filename, std::ios::out | std::ios::binary);

	uart_reader.init ("/dev/ttyMFD1");

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);

	parser.register_handler ([this] (DataBuffer buffer) {
		file.write ((char*)&buffer.timestamp, sizeof(buffer.timestamp));
		file.write ((char*)buffer.data, buffer.size);
	});
}

void DataReader::start()
{
	std::lock_guard<std::mutex> lock (start_stop);
	run = true;
	char tmp_buffer [128];
	while (run)
	{
		auto size = uart_reader.read_data (tmp_buffer, 128);
		parser.append_bytes (tmp_buffer, size);
	}
	close();
}

void DataReader::close()
{
	Logger::log ("closing file ", filename);
	file.close ();
	uart_reader.close ();
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

