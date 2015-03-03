#include "client_service.h"
#include "logger.h"

#include <sys/socket.h>
#include <unistd.h>

using namespace SnowCookie;

ClientService::ClientService(int sock_fd)
 : sock_fd(sock_fd)
{
}

void ClientService::service(std::function<void()> finish_handler)
{
	Logger::log ("starting client service");

	char buff[max_buffer_size] = {0};
	int n;
	while ((n = recv (sock_fd, buff, max_buffer_size, 0)) > 0)
	{
		send (sock_fd, buff, n, 0);
	}
	close ();
	finish_handler();
}

void ClientService::close()
{
	Logger::log ("closing connection with client");
	::close(sock_fd);
}
