#include "service_manager.h"
#include "logger.h"

#include <thread>

using namespace SnowCookie;

void ServiceManager::register_service(std::shared_ptr<BgService> service, ServiceType type)
{
	Logger::log ("register service ", service->get_name());
	services.insert (std::make_pair (type, service));
}

bool ServiceManager::init_all()
{
	Logger::log ("initializing services");
	bool init = true;

	for (auto service : services)
	{
		try
		{
			Logger::log ("Trying to init service ", service.second->get_name());
			service.second->init();
		}
		catch (const std::exception& ex)
		{
			Logger::log("Exception handled: " , ex.what());
			init = false;
		}
	}

	Logger::log ("initialization completed, result: ", init);
	return init;
}

void ServiceManager::stop_all()
{
	Logger::log ("stop services");

	for (auto service : services)
	{
		Logger::log ("trying to stop service", service.second->get_name());
		service.second->stop();
	}
}

void ServiceManager::start_all()
{
	Logger::log ("starting services");

	for (auto service : services)
	{
		Logger::log ("trying to run service ", service.second->get_name());

		std::thread([service] {
			service.second->start();
		}).detach();
	}
	Logger::log ("running finished");
}
