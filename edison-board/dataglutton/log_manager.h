/*
 * log_manager.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_LOG_MANAGER_H_
#define EDISON_BOARD_DATAGLUTTON_LOG_MANAGER_H_

#include "protocol/data_parser.h"
#include "divisor.h"

#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <map>

namespace SnowCookie {

class LogManager
{
	std::vector<std::string> log_files;

	std::map<int, std::function<void(DataBuffer)>> handlers;

	bool paused = false;
	std::ofstream file;
	Divisor divisor;

public:
	LogManager ();
	virtual ~LogManager () {}
	void start_log (const std::string& filename);
	void pause_log ();
	void resume_log ();
	void stop_log ();
	void write (DataBuffer buff);
	int get_log_count () const;
	void remove_logs ();

	void register_log_handler (int num, std::function<void(DataBuffer)> fun);
	void unregister_handler (int num);

	static std::string generate_unique_filename (std::string name);
	static std::string get_dated_filename ();

};

}

#endif /* EDISON_BOARD_DATAGLUTTON_LOG_MANAGER_H_ */
