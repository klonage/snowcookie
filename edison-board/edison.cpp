#include "server/server.h"
#include "service_manager.h"
#include "logger.h"
#include "dataglutton/log_manager.h"

#include <thread>
#include <condition_variable>
#include <mutex>
#include <ctime>

#include <signal.h>
#include "dataglutton/data_writer.h"

using namespace SnowCookie;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void signal_handler(int sig)
{
	Logger::log ("gotcha");
	std::unique_lock<std::mutex> lck(mtx);
	ready = true;
	cv.notify_all();
}

int main(int argc, char** argv)
{
	std::shared_ptr<ServiceManager> manager = std::make_shared<ServiceManager> ();
	int port = 12345;

	if (argc > 1)
	{
		port = atoi(argv[1]);
	}

	Logger::log ("Port: ", port);

	auto log_manager = std::make_shared<LogManager> ();

	manager->register_service(std::make_shared<Server>(port, log_manager, manager), ServiceType::TCP_SERVER);
	manager->register_service(std::make_shared<DataWriter>(log_manager, manager), ServiceType::UART_DEVICE);

	signal(SIGINT, signal_handler);

	if (!manager->init_all())
	{
		return 0;
	}

	manager->start_all();

	std::thread([&manager]{
		std::unique_lock<std::mutex> lck(mtx);
		while (!ready) cv.wait(lck);
		manager->stop_all();
	}).join();

	return 0;
}
