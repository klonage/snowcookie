#ifndef _SNOWCOOKIE_DATA_READER_H
#define _SNOWCOOKIE_DATA_READER_H

#include "bg_service.h"

#include "data_parser.h"
#include "uart_device.h"

#include <fstream>
#include <string>

namespace SnowCookie {

class DataWriter : public BgService
{
	std::ofstream file;
	std::string filename;

	UartDevice uart_reader;
	DataParser parser;

	void close();

public:
	DataWriter(const std::string& filename);
	virtual ~DataWriter() {}

	void init() override;
	void start() override;
	void stop() override;

	std::string get_name() const override
	{ return std::string("data reader"); }

	// todo to another class
	static std::string generate_unique_filename(std::string name);
};

}
#endif
