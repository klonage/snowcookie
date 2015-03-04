#include "server/server.h"
#include "service_manager.h"
#include "logger.h"

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

std::string get_dated_filename()
{
	std::time_t t = std::time(NULL);
	char mbstr[30];
	if (std::strftime(mbstr, sizeof(mbstr), "log_%Y-%m-%d-%H-%M-%S.txt", std::localtime(&t))) {
		return std::string(mbstr);
	}
	return "log_unknown";
}

int main(int argc, char** argv)
{
	ServiceManager manager;
	int port = 12345;
	std::string filename = get_dated_filename();

	if (argc > 1)
	{
		port = atoi(argv[1]);

		if (argc > 2)
		{
			filename = argv[2];
		}
	}

	filename = DataWriter::generate_unique_filename(filename);

	Logger::log ("Port: ", port);
	Logger::log ("Destination file name: ", filename);

	manager.register_service(std::make_shared<Server>(port));
	manager.register_service(std::make_shared<DataWriter>(filename));

	signal(SIGINT, signal_handler);

	if (!manager.init_all())
	{
		return 0;
	}

	manager.start_all();

	std::thread([&manager]{
		std::unique_lock<std::mutex> lck(mtx);
		while (!ready) cv.wait(lck);
		manager.stop_all();
	}).join();

	return 0;
}
