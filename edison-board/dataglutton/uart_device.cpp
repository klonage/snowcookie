/*
 * uart_device.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#include "logger.h"

#include <unistd.h>
#include <fcntl.h>

#include <stdexcept>
#include "uart_device.h"

using namespace SnowCookie;

void UartDevice::init (const std::string& dev_name)
{
	uart_fd = open(dev_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	if (uart_fd == -1)
		throw std::runtime_error ("cannot open uart device");

	tcgetattr(uart_fd, &options);
	options.c_cflag = B460800 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart_fd, TCIFLUSH);
	tcsetattr(uart_fd, TCSANOW, &options);

	Logger::log ("uart initialized");
}

int UartDevice::read_data (char buffer[], int max_size)
{
	return read (uart_fd, buffer, max_size);
}

void UartDevice::close ()
{
	if (uart_fd  < 0)
		return;

	::close (uart_fd);
}

int UartDevice::write_data (char buffer[], int size)
{
	return write (uart_fd, buffer, size);
}

