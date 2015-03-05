#include "server.h"
#include "client_service.h"
#include "logger.h"
#include "service_manager.h"
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <cassert>

#include <thread>
#include "../dataglutton/iplug_device.h"

using namespace SnowCookie;

Server::Server(int port, std::shared_ptr<ServiceManager> manager)
: SnowCookie::BgService (manager), port(port)
{
}

void Server::init()
{
	Logger::log ("initializing server");

	sock_fd = socket (AF_INET, SOCK_STREAM, 0);

	if (sock_fd < 0)
		throw std::runtime_error ("cannot open socket");

	sockaddr_in serv_addr;
	memset ((char*) &serv_addr, 0, sizeof (sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons (port);

	if (bind (sock_fd, (sockaddr*) &serv_addr, sizeof (serv_addr)) < 0)
		throw std::runtime_error ("cannot bind");
}

void Server::start ()
{
	Logger::log ("running server");

	listen (sock_fd, max_connections);

	socklen_t sockaddr_len = sizeof (sockaddr);
	sockaddr_in cli_addr;
	memset ((char*) &cli_addr, 0, sizeof (sockaddr_in));

	run = true;
	thread_count = 0;

	std::lock_guard<std::mutex> lock (start_stop);

	while (run)
	{
		int client_sock_fd = accept (sock_fd, (sockaddr*) &cli_addr, &sockaddr_len);
		if (client_sock_fd >= 0)
		{
			request_handler (client_sock_fd);
		}
		else
		{
			Logger::log ("cannot accept socket");
		}
	}

	Logger::log ("server stopped, threads: ", thread_count);
}

void Server::request_handler(int client_fd)
{
	Logger::log ("new client");

	thread_count++;
	std::thread t([client_fd, this] {
		ClientService(client_fd).service([this]{thread_finished();});
	});
	t.detach();
}

void Server::stop()
{
	run = false;
	::shutdown(sock_fd, SHUT_RDWR);
	std::lock_guard<std::mutex> lock (start_stop);
}

void Server::thread_finished()
{
	assert(thread_count);
	thread_count--;

	Logger::log ("thread finished, ", thread_count, " thread(s) left");
}

void Server::pass_to_device (unsigned char *buffer, int size)
{
	auto device = manager->get_service<IPlugDevice> (ServiceType::UART_DEVICE);
	if (device)
	{
		int s = size;
		while (s > 0)
		{
			auto x = device->write_data (buffer, size);
			if (x < 0)
				break;
			s -= x;
		}
	}
}
