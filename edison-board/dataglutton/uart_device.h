/*
 * uart_device.h
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_UART_DEVICE_H_
#define EDISON_BOARD_DATAGLUTTON_UART_DEVICE_H_

#include <termios.h>

#include <string>

#include "idata_provider.h"

namespace SnowCookie {

class UartDevice : public IDataProvider
{
	termios options;
	int uart_fd = -1;

public:
	virtual ~UartDevice () {}

	void init (const std::string& dev_name) override;
	void close () override;
	int read_data (char buffer[], int max_size) override;
	int write_data (unsigned char buffer[], int size) override;
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_UART_DEVICE_H_ */
