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
	// todo !!!
	const unsigned char end_character = 0x0A;
	const char substitute_character = 0x1A;
	const char xor_character = 0x33;
	auto frame = EdisonFrame::parse_frame (buffer.frame, buffer.frame_size);
	switch (frame->get_type ())
	{
	case EdisonFrame::GET_STATUS:
	{
		GetStatusEdisonFrame f (false); f.set_data (10);
		unsigned char dest [128];
		unsigned char d [128]; unsigned long int s = f.serialize(d);
		s = pack_frame (d, s, dest, substitute_character, xor_character, end_character);
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
