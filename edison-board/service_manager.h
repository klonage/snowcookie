#ifndef _SNOWCOOKIE_SERVICE_MANAGER_H
#define _SNOWCOOKIE_SERVICE_MANAGER_H

#include "bg_service.h"

#include <map>
#include <memory>

namespace SnowCookie {

enum class ServiceType
{
	TCP_SERVER,
	UART_DEVICE,
	WIFI_SERVICE
};

class ServiceManager
{
	std::map<ServiceType, std::shared_ptr<BgService>> services;

public:
	virtual ~ServiceManager() {}

	void register_service(std::shared_ptr<BgService> service, ServiceType type);
	bool init_all();
	void start_all();
	void stop_all();

	template <typename T>
	std::shared_ptr<T> get_service (ServiceType type)
	{
		return std::dynamic_pointer_cast<T> (services [type]);
	}
};

}

#endif
