#ifndef _SNOWCOOKIE_SERVER_H
#define _SNOWCOOKIE_SERVER_H

#include "bg_service.h"

#include "protocol/data_parser.h"

#include <atomic>

namespace SnowCookie {

class Server : public BgService
{
	std::atomic_int thread_count;
	int sock_fd = -1;
	int port;

	static constexpr int max_buffer_size = 256;
	static constexpr int max_connections = 5;

	void request_handler(int client_fd);
	void thread_finished();
public:
	Server(int port, std::shared_ptr<ServiceManager> manager);
	virtual ~Server(){}

	void init() override;
	void start() override;
	void stop() override;

	std::string get_name() const override
	{ return std::string("tcp server"); }

	void pass_to_device (char *buffer, int size);
};

}

#endif
