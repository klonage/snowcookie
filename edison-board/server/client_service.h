#ifndef _SNOWCOOKIE_CLIENT_SERVICE_H
#define _SNOWCOOKIE_CLIENT_SERVICE_H

#include "server.h"

#include <functional>

namespace SnowCookie {

class ClientService
{
private:
	static constexpr int max_buffer_size = 256;
	int sock_fd;

	void close();

public:
	ClientService(int sock_fd);
	virtual ~ClientService() {}

	void service(std::function<void()> finish_handler);
};

}

#endif
