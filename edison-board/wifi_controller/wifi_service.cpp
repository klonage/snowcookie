/*
 * wifi_controller.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: loganek
 */

#include "wifi_service.h"

#include "logger.h"

#include "server/server.h"

#include <sstream>
#include <cstdio>

using namespace SnowCookie;


std::string WifiService::run_command (const std::string& cmd)
{
	Logger::log ("running command ", cmd);
	FILE* pipe = popen(cmd.c_str (), "r");
	if (!pipe) return std::string();
	char buffer[128];
	std::string result = "";
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

int WifiService::get_wpa_process_pid ()
{
	auto result = run_command ("ps | grep /usr/sbin/wpa_supplicant | grep -v grep");

	if (result.length () == 0)
	{
		Logger::log ("wpa process not found, return -1");
		return -1;
	}

	int pid;
	std::stringstream stream(result);
	stream >> pid;

	Logger::log ("wpa process found, pid: ", pid);

	return pid;
}

void WifiService::start ()
{
	std::lock_guard<std::mutex> lock (start_stop);

	run = true;

	while (run)
	{
		std::unique_lock<std::mutex> lk(cv_m);
		if(cv.wait_for(lk, std::chrono::minutes(5)) == std::cv_status::no_timeout)
		{
			Logger::log ("Broke by user");
			break;
		}

		if (manager->get_service<Server> (ServiceType::TCP_SERVER)->get_connected_clients () > 0)
			continue;
		Logger::log ("clients not connected to server");

		int pid = get_wpa_process_pid ();
		if (pid != -1)
		{
			std::string cmd = "kill ";
			cmd += std::to_string (pid);
			run_command (cmd);
			Logger::log ("kill wpa_supplicant");
		}
		else
		{
			Logger::log ("Wifi already disabled");
		// 	run_command ("configure_edison --disableOneTimeSetup");
		}
	}
	Logger::log ("Wifi service end start");
}

void WifiService::stop ()
{
	run = false;
	cv.notify_all ();
	std::lock_guard<std::mutex> lock (start_stop);
}

