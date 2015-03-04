#include "data_writer.h"
#include "logger.h"
#include "uart_device.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <sys/stat.h>

using namespace SnowCookie;

DataWriter::DataWriter(const std::string& filename)
 : filename(filename),
   parser(0x0A, 0x1A, 0x33)
{
	data_provider = std::make_shared<UartDevice> ();
}

void DataWriter::init()
{
	file.open(filename, std::ios::out | std::ios::binary);

	data_provider->init ("/dev/ttyMFD1");

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);

	parser.register_handler ([this] (DataBuffer buffer) {
		file.write ((char*)&buffer.timestamp, sizeof(buffer.timestamp));
		file.write ((char*)buffer.data, buffer.size);
	});
}

void DataWriter::start()
{
	std::lock_guard<std::mutex> lock (start_stop);
	run = true;
	char tmp_buffer [128];
	while (run)
	{
		auto size = data_provider->read_data (tmp_buffer, 128);
		parser.append_bytes (tmp_buffer, size);
	}
	close();
}

void DataWriter::close()
{
	Logger::log ("closing file ", filename);
	file.close ();
	data_provider->close ();
}

void DataWriter::stop()
{
	run = false;
	std::lock_guard<std::mutex> lock (start_stop);
}

std::string DataWriter::generate_unique_filename(std::string name)
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

