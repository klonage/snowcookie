/*
 * idata_provider.h
 *
 *  Created on: Mar 4, 2015
 *      Author: loganek
 */

#ifndef EDISON_BOARD_DATAGLUTTON_IDATA_PROVIDER_
#define EDISON_BOARD_DATAGLUTTON_IDATA_PROVIDER_

#include <string>

namespace SnowCookie {

class IDataProvider
{
public:
	virtual ~IDataProvider () {}

	virtual int read_data (char buffer[], int max_size) = 0;
	virtual int write_data (unsigned char buffer[], int size) = 0;
	virtual void init (const std::string& source) = 0;
	virtual void close () = 0;
};

}

#endif /* EDISON_BOARD_DATAGLUTTON_IDATA_PROVIDER_ */
