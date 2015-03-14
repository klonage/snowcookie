/*
 * wifi_service.h
 *
 *  Created on: Mar 14, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_WIFI_CONTROLLER_WIFI_SERVICE_H_
#define EDISON_BOARD_WIFI_CONTROLLER_WIFI_SERVICE_H_

#include "bg_service.h"

#include "service_manager.h"
#include <condition_variable>
#include <string>

namespace SnowCookie {

class WifiService : public BgService
{
private:
	std::condition_variable cv;
	std::mutex cv_m;
	static std::string run_command (const std::string& cmd);
	static int get_wpa_process_pid ();

public:
	WifiService (std::shared_ptr<ServiceManager> manager) : BgService (manager) {}
	virtual ~WifiService () {}
	void init() override {}
	void start() override;
	void stop() override;
	std::string get_name() const override { return "Wifi switcher"; }
};

}

#endif /* EDISON_BOARD_WIFI_CONTROLLER_WIFI_SERVICE_H_ */
