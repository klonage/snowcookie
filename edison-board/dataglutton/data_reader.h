#ifndef _SNOWCOOKIE_DATA_READER_H
#define _SNOWCOOKIE_DATA_READER_H

#include "bg_service.h"

#include <fstream>
#include <string>

namespace SnowCookie {

class DataReader : public BgService
{
	std::ofstream file;
	std::string filename;

	void close();

public:
	DataReader(const std::string& filename);
	virtual ~DataReader() {}

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
