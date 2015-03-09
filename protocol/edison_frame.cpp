/*
 * edison_frame.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "edison_frame.h"

#include <cstring>
#include <sys/statvfs.h>
#include <cassert>

using namespace SnowCookie;

std::shared_ptr<EdisonFrame> EdisonFrame::parse_frame (unsigned char * buffer, int size)
{
	assert (size > 0);

	switch (buffer [0])
	{
	case GET_STATUS:
		return std::make_shared <GetStatusEdisonFrame> (buffer, size);
	default:
		throw std::runtime_error ("invalid frame type");
	}
}

int EdisonFrame::serialize (unsigned char* data)
{
	data [0] = frame_type;
	data [1] = err_flag;

	return 2;
}

GetStatusEdisonFrame::GetStatusEdisonFrame (unsigned char* buffer, int size)
: EdisonFrame (GET_STATUS)
{
	assert (size > 1);
	request = buffer [1];

	if (request)
		return;
}

void GetStatusEdisonFrame::set_data (int log_count)
{
	struct statvfs fiData;

	// todo check directory
	if ((statvfs ("/",&fiData)) < 0)
		err_flag = true;
	else
		size = fiData.f_bsize * fiData.f_bfree;
}

int GetStatusEdisonFrame::serialize (unsigned char* data)
{
	int curr_ptr = EdisonFrame::serialize (data);
	data [curr_ptr++] = request;
	if (request)
		return curr_ptr;

	memcpy (data + curr_ptr, (char*)&size, sizeof (size)); curr_ptr += sizeof (size);

	return curr_ptr;
}
