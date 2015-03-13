#include "data_writer.h"
#include "logger.h"
#include "uart_device.h"
#include "fake_data_provider.h"
#include "service_manager.h"

#include <iostream>
#include <thread>

using namespace SnowCookie;
//#define FAKE_DATA
DataWriter::DataWriter(std::shared_ptr<LogManager> log_manager, std::shared_ptr<ServiceManager> manager)
 : BgService (manager),
   log_manager (log_manager)
{
#ifdef FAKE_DATA
	data_provider = std::make_shared<FakeDataProvider> ();
#else
	data_provider = std::make_shared<UartDevice> ();
#endif
}

void DataWriter::init()
{
	data_provider->init ("/dev/ttyMFD1");

	parser.register_handler ([this] (DataBuffer buffer) {
		log_manager->write (buffer);
	});
}

void DataWriter::start()
{
	std::lock_guard<std::mutex> lock (start_stop);
	log_manager->start_log (LogManager::generate_unique_filename (LogManager::get_dated_filename ()));
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
	Logger::log ("closing file ");
	log_manager->stop_log ();
	data_provider->close ();
}

void DataWriter::stop()
{
	run = false;
	std::lock_guard<std::mutex> lock (start_stop);
}

