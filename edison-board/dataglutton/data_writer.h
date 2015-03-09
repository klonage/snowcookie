#ifndef _SNOWCOOKIE_DATA_READER_H
#define _SNOWCOOKIE_DATA_READER_H

#include "bg_service.h"
#include "log_manager.h"

#include "protocol/data_parser.h"
#include <fstream>
#include <memory>
#include <string>

#include "iplug_device.h"

namespace SnowCookie {

class DataWriter : public BgService
{
	std::shared_ptr<IPlugDevice> data_provider;
	std::shared_ptr<LogManager> log_manager;
	DataParser parser;

	void close();

public:
	DataWriter(std::shared_ptr<LogManager> log_manager, std::shared_ptr<ServiceManager> manager);
	virtual ~DataWriter() {}

	void init() override;
	void start() override;
	void stop() override;

	std::string get_name() const override
	{ return std::string("data reader"); }
};

}
#endif
