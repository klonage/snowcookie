#ifndef _SNOWCOOKIE_SERVER_H
#define _SNOWCOOKIE_SERVER_H

#include "bg_service.h"

#include "protocol/data_parser.h"

#include "dataglutton/log_manager.h"

#include <atomic>
#include <memory>

namespace SnowCookie {

class Server : public BgService, public std::enable_shared_from_this<Server>
{
	std::atomic_int thread_count;
	int sock_fd = -1;
	int port;

	static constexpr int max_buffer_size = 256;
	static constexpr int max_connections = 5;

	std::shared_ptr<LogManager> log_manager;

	void request_handler(int client_fd);
	void thread_finished();
public:
	Server(int port, std::shared_ptr<LogManager> log_manager, std::shared_ptr<ServiceManager> manager);
	virtual ~Server(){}

	void init() override;
	void start() override;
	void stop() override;

	std::string get_name() const override
	{ return std::string("tcp server"); }

	void pass_to_device (char *buffer, int size);

	int get_connected_clients () const { return thread_count; }
};

}

#endif
