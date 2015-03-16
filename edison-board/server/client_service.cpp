#include "client_service.h"
#include "logger.h"
#include "protocol/edison_frame.h"
#include "protocol/protocol_utils.h"

#include <sys/socket.h>
#include <unistd.h>

using namespace SnowCookie;

ClientService::ClientService(std::shared_ptr<LogManager> log_manager, std::shared_ptr<Server> parent_server, int sock_fd)
: sock_fd(sock_fd), divisor({{0x10, 0}, {0x11, 0}, {0x12, 0}, {0x13, 1}}), log_manager(log_manager), server (parent_server)
{
	parser.register_handler ([this] (DataBuffer buffer) {
		on_buffer_parsed (buffer);
	});

	log_manager->register_log_handler (sock_fd, [this](DataBuffer buffer) {
		divisor.update (buffer.frame [1]);
		if (!divisor.can_push (buffer.frame[1]))
			return;
		write (this->sock_fd, buffer.data, buffer.size);
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
	try
	{
		auto frame = EdisonFrame::parse_frame (buffer.frame, buffer.frame_size);
		switch (frame->get_type ())
		{
		case EdisonFrame::GET_STATUS:
		{
			GetStatusEdisonFrame f (false);
			f.set_data (log_manager->get_log_count ());
			char dest [128];
			int s = EdisonFrame::pack_and_serialize(f, dest);
			send (sock_fd, dest, s, 0);
			break;
		}
		case EdisonFrame::CLEAR_LOGS:
			log_manager->remove_logs ();
			break;
		case EdisonFrame::STOP_LOG:
			log_manager->stop_log ();
			break;
		case EdisonFrame::START_LOG:
			log_manager->start_log (LogManager::get_dated_filename ());
			break;
		case EdisonFrame::WIFI:
			server->stop_wifi_disabling ();
			break;
		case EdisonFrame::DIVISOR:
		{
			auto f = std::static_pointer_cast<DivisorEdisonFrame> (frame);
			if (f->get_dest () == 1)
				log_manager->update_divisor (f->get_location(), f->get_divisor());
			else if (f->get_dest () == 0)
				divisor.set_division (f->get_location(), f->get_divisor());
			break;
		}
		case EdisonFrame::STM_PASS: // to stm
			server->pass_to_device (buffer.data, buffer.size);
			break;
		default:
			break;// edison handler
		}
	}
	catch (const std::exception& ex)
	{
		Logger::log ("exception during processing frame: ", ex.what());
	}
}
