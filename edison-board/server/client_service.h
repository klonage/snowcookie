#ifndef _SNOWCOOKIE_CLIENT_SERVICE_H
#define _SNOWCOOKIE_CLIENT_SERVICE_H

#include "server.h"

#include "dataglutton/divisor.h"
#include "protocol/data_parser.h"

#include <functional>

namespace SnowCookie {

class ClientService
{
private:
	static constexpr int max_buffer_size = 256;
	int sock_fd;

	Divisor divisor;
	DataParser parser;
	std::shared_ptr<LogManager> log_manager;
	std::shared_ptr<Server> server;

	void close();
	void on_buffer_parsed (const DataBuffer& buffer);

public:
	ClientService(std::shared_ptr<LogManager> log_manager, std::shared_ptr<Server> parent_server, int sock_fd);
	virtual ~ClientService() {log_manager->unregister_handler(sock_fd);}

	void service(std::function<void()> finish_handler);
};

}

#endif
