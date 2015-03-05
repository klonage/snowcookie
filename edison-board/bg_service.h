#ifndef _SNOWCOOKIE_BG_SERVICE_H
#define _SNOWCOOKIE_BG_SERVICE_H

#include <mutex>
#include <atomic>
#include <memory>

namespace SnowCookie {

class ServiceManager;

class BgService
{
protected:
	std::atomic_bool run;
	std::mutex start_stop;

	std::shared_ptr<ServiceManager> manager;

public:
	BgService (std::shared_ptr<ServiceManager> manager) : manager (manager) {}
	virtual ~BgService() {}

	virtual void init() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual std::string get_name() const = 0;
};

}

#endif
