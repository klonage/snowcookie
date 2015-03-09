/*
 * edison_frame.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "edison_frame.h"

#include "protocol/protocol_utils.h"

#include <cstring>
#include <sys/statvfs.h>
#include <cassert>

using namespace SnowCookie;

std::shared_ptr<EdisonFrame> EdisonFrame::parse_frame (char * buffer, int size)
{
	assert (size > 2);

	switch (buffer [0])
	{
	case GET_STATUS:
	{
		auto f = std::make_shared <GetStatusEdisonFrame> (buffer, size);
		f->err_flag = buffer [1];
		return f;
	}
	default:
		throw std::runtime_error ("invalid frame type");
	}
}

int EdisonFrame::serialize (char* data) const
{
	data [0] = frame_type;
	data [1] = err_flag;

	return 2;
}


int EdisonFrame::pack_and_serialize (const EdisonFrame& frame, char* data)
{
	// todo magic number
	char d [256];
	int size = frame.serialize (d);
	return pack_frame (d, size, data);
}

GetStatusEdisonFrame::GetStatusEdisonFrame (char* buffer, int buffer_size)
: EdisonFrame (GET_STATUS)
{
	request = buffer [2];

	if (request)
		return;

	// todo might be incorrect for some platforms
	memcpy ((char*)& size, buffer + 3, sizeof (size));
	memcpy ((char*)& log_count, buffer + 3 + sizeof (size), sizeof (log_count));
}

void GetStatusEdisonFrame::set_data (int log_count)
{
	struct statvfs fiData;

	// todo check directory
	if ((statvfs ("/",&fiData)) < 0)
		err_flag = true;
	else
		size = fiData.f_bsize * fiData.f_bfree;
	this->log_count = log_count;
}

int GetStatusEdisonFrame::serialize (char* data) const
{
	int curr_ptr = EdisonFrame::serialize (data);
	data [curr_ptr++] = request;
	if (request)
		return curr_ptr;

	memcpy (data + curr_ptr, (char*)&size, sizeof (size)); curr_ptr += sizeof (size);
	memcpy (data + curr_ptr, (char*)&log_count, sizeof (log_count)); curr_ptr += sizeof (log_count);

	return curr_ptr;
}
