#include "client_service.h"
#include "logger.h"
#include "protocol/edison_frame.h"
#include "protocol/protocol_utils.h"

#include <sys/socket.h>
#include <unistd.h>

using namespace SnowCookie;

ClientService::ClientService(int sock_fd)
 : sock_fd(sock_fd)
{
	parser.register_handler ([this] (DataBuffer buffer) {
		on_buffer_parsed (buffer);
	});
}

void ClientService::service(std::function<void()> finish_handler)
{
	Logger::log ("starting client service");

	char buff[max_buffer_size] = {0};
	int n;
	while ((n = recv (sock_fd, buff, max_buffer_size, 0)) > 0)
	{
		parser.append_bytes(buff, n);
	}
	close ();
	finish_handler();
}

void ClientService::close()
{
	Logger::log ("closing connection with client");
	::close(sock_fd);
}

void ClientService::on_buffer_parsed (const DataBuffer& buffer)
{
	auto frame = EdisonFrame::parse_frame (buffer.frame, buffer.frame_size);
	switch (frame->get_type ())
	{
	case EdisonFrame::GET_STATUS:
	{
		GetStatusEdisonFrame f (false);
		f.set_data (10);
		char dest [128];
		int s = EdisonFrame::pack_and_serialize(f, dest);
		send (sock_fd, dest, s, 0);
		break;
	}
	case 'S': // to stm
		server->pass_to_device (buffer.frame, buffer.frame_size);
		break;
	default:
		break;// edison handler
	}
}
