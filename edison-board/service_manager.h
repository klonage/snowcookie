#ifndef _SNOWCOOKIE_SERVICE_MANAGER_H
#define _SNOWCOOKIE_SERVICE_MANAGER_H

#include "bg_service.h"

#include <vector>
#include <memory>

namespace SnowCookie {

class ServiceManager
{
	std::vector<std::shared_ptr<BgService>> services;

public:
	virtual ~ServiceManager() {}

	void register_service(std::shared_ptr<BgService> service);
	bool init_all();
	void start_all();
	void stop_all();
};

}

#endif
