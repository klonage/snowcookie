#include "data_writer.h"
#include "logger.h"
#include "uart_device.h"
#include "fake_data_provider.h"
#include "service_manager.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <sys/stat.h>

using namespace SnowCookie;
#define FAKE_DATA
DataWriter::DataWriter(const std::string& filename, std::shared_ptr<ServiceManager> manager)
 : BgService (manager),
   filename(filename)
{
#ifdef FAKE_DATA
	data_provider = std::make_shared<FakeDataProvider> ();
#else
	data_provider = std::make_shared<UartDevice> ();
#endif
}

void DataWriter::init()
{
	file.open(filename, std::ios::out | std::ios::binary);

	data_provider->init ("/dev/ttyMFD1");

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);

	parser.register_handler ([this] (DataBuffer buffer) {
		auto now = std::chrono::high_resolution_clock::now ();
		long long timestamp = std::chrono::duration_cast<std::chrono::microseconds> (now.time_since_epoch ()).count ();
		file.write ((char*)&timestamp, sizeof(timestamp));
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
	file.flush (); // <- because of compiler's bugs
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

