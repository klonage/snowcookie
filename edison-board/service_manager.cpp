#include "service_manager.h"
#include "logger.h"

#include <thread>

using namespace SnowCookie;

void ServiceManager::register_service(std::shared_ptr<BgService> service)
{
	Logger::log ("register service ", service->get_name());
	services.push_back(service);
}

bool ServiceManager::init_all()
{
	Logger::log ("initializing services");
	bool init = true;

	for (auto service : services)
	{
		try
		{
			Logger::log ("Trying to init service ", service->get_name());
			service->init();
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
		Logger::log ("trying to stop service", service->get_name());
		service->stop();
	}
}

void ServiceManager::start_all()
{
	Logger::log ("starting services");

	for (auto service : services)
	{
		Logger::log ("trying to run service ", service->get_name());

		std::thread([service] {
			service->start();
		}).detach();
	}
	Logger::log ("running finished");
}
