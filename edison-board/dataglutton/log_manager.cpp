/*
 * log_manager.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "log_manager.h"
#include "logger.h"

#include <unistd.h>
#include <sys/stat.h>

#include <cstring>

#include <stdexcept>
#include <chrono>

using namespace SnowCookie;

LogManager::LogManager ()
: divisor ({})
{}

int LogManager::get_log_count () const { return log_files.size (); }

void LogManager::start_log (const std::string& filename)
{
	if (file.is_open ())
		return;

	file.open(filename, std::ios::out | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("cannot open file: " + filename);

	Logger::log ("start loggin in file ", filename);
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
	divisor.update (buffer.frame [1]);
	for (auto h : handlers)
		h.second (buffer);

	if (paused)
		return;

	if (!divisor.can_push (buffer.frame [1]))
		return;

	auto now = std::chrono::high_resolution_clock::now ();
	long long timestamp = std::chrono::duration_cast<std::chrono::microseconds> (now.time_since_epoch ()).count ();
	file.write ((char*)&timestamp, sizeof(timestamp));
	file.write ((char*)buffer.frame, buffer.frame_size);
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

static int do_mkdir(const char *path, mode_t mode)
{
	struct stat st;
	int status = 0;

	if (stat(path, &st) != 0)
	{
		if (mkdir (path, mode) != 0 && errno != EEXIST)
			status = -1;
	}
	else if (!S_ISDIR (st.st_mode))
	{
		errno = ENOTDIR;
		status = -1;
	}

	return status;
}

static int mkpath(const char *path, mode_t mode)
{
	char *pp;
	char *sp;
	int status;
	char *copypath = strdup(path);

	status = 0;
	pp = copypath;
	while (status == 0 && (sp = strchr(pp, '/')) != 0)
	{
		if (sp != pp)
		{
			*sp = '\0';
			status = do_mkdir(copypath, mode);
			*sp = '/';
		}
		pp = sp + 1;
	}
	if (status == 0)
		status = do_mkdir(path, mode);
	free (copypath);
	return status;
}

std::string LogManager::get_dated_filename ()
{
	// todo quick hack, Intel EDISON & Linux Yocto only!
	char mbstr[64];
	int ctr = 0;
	std::string dir = "/home/root/edison_logs/";
	int status = mkpath (dir.c_str(), 0777);
	if (status == 0)
	{
		ctr = snprintf (mbstr, 64, "%s", dir.c_str());
	}

	std::time_t t = std::time(NULL);
	if (std::strftime(mbstr + ctr, sizeof(mbstr) - ctr, "log_%Y-%m-%d-%H-%M-%S.txt", std::localtime(&t))) {
		return std::string(mbstr);
	}
	return "log_unknown";
}

void LogManager::update_divisor (int location, int divisor)
{
	this->divisor.set_division(location, divisor);
}
