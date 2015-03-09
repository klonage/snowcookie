/*
 * log_manager.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "log_manager.h"

#include <unistd.h>
#include <sys/stat.h>

#include <stdexcept>
#include <chrono>

using namespace SnowCookie;

LogManager::LogManager ()
: divisor (1, 1, 1)
{}

int LogManager::get_log_count () const { return log_files.size (); }

void LogManager::start_log (const std::string& filename)
{
	if (file.is_open ())
		return;

	file.open(filename, std::ios::out | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);

	log_files.push_back (filename);

	paused = false;
}

void LogManager::stop_log ()
{
	if (!file.is_open ())
		return;

	file.flush (); // <- because of compiler's bugs
	file.close ();
}

void LogManager::pause_log ()
{
	paused = true;
}

void LogManager::resume_log ()
{
	paused = false;
}

void LogManager::register_log_handler (int num, std::function<void(DataBuffer)> fun)
{
	handlers [num] = fun;
}

void LogManager::unregister_handler (int num)
{
	handlers.erase (num);
}

void LogManager::write (DataBuffer buffer)
{
	divisor.update (buffer.frame [0]);
	for (auto h : handlers)
		h.second (buffer);

	if (paused)
		return;

	if (!divisor.can_push (buffer.frame [0]))
		return;

	auto now = std::chrono::high_resolution_clock::now ();
	long long timestamp = std::chrono::duration_cast<std::chrono::microseconds> (now.time_since_epoch ()).count ();
	file.write ((char*)&timestamp, sizeof(timestamp));
	file.write ((char*)buffer.data, buffer.size);
}

void LogManager::remove_logs ()
{
	// cannot remove on recording (?)
	if (file.is_open ())
		return;

	for (auto file : log_files)
		unlink (file.c_str ());

	log_files.clear ();
}

std::string LogManager::generate_unique_filename(std::string name)
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

std::string LogManager::get_dated_filename ()
{
	std::time_t t = std::time(NULL);
	char mbstr[30];
	if (std::strftime(mbstr, sizeof(mbstr), "log_%Y-%m-%d-%H-%M-%S.txt", std::localtime(&t))) {
		return std::string(mbstr);
	}
	return "log_unknown";
}

void LogManager::update_divisor (int location, int divisor)
{
	this->divisor.set_division(location, divisor);
}
