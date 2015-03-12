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
	assert (size > 1);

	switch (buffer [0])
	{
	case GET_STATUS:
		return std::make_shared <GetStatusEdisonFrame> (buffer, size);
	case CLEAR_LOGS:
	case START_LOG:
	case STOP_LOG:
		return std::make_shared <SimpleLogEdisonFrame> ((Type)buffer[0]);
	case DIVISOR:
		return std::make_shared <DivisorEdisonFrame> (buffer, size);
	case STM_PASS:
		return std::make_shared <StmPassEdisonFrame> ();
	default:
		throw std::runtime_error ("invalid frame type");
	}
}

int EdisonFrame::serialize (char* data) const
{
	data [0] = frame_type;

	return 1;
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
	request = buffer [1];

	if (request)
		return;

	// todo might be incorrect for some platforms
	memcpy ((char*)& size, buffer + 2, sizeof (size));
	memcpy ((char*)& log_count, buffer + 2 + sizeof (size), sizeof (log_count));
}

void GetStatusEdisonFrame::set_data (int log_count)
{
	struct statvfs fiData;
	statvfs ("/home",&fiData);
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

int SimpleLogEdisonFrame::serialize (char* data) const
{
	return EdisonFrame::serialize (data);
}

DivisorEdisonFrame::DivisorEdisonFrame (char* buffer, int size)
 : EdisonFrame (DIVISOR)
{
	dest = buffer[1];
	memcpy ((char*)& location, buffer + 2, sizeof (location));
	memcpy ((char*)& divisor, buffer + 2 + sizeof(location), sizeof (divisor));
}

int DivisorEdisonFrame::serialize (char* data) const
{
	int curr_ptr = EdisonFrame::serialize (data);
	data[curr_ptr++] = dest;
	memcpy (data + curr_ptr, (char*)&location, sizeof (location)); curr_ptr += sizeof (location);
	memcpy (data + curr_ptr, (char*)&divisor, sizeof (divisor)); curr_ptr += sizeof (divisor);

	return curr_ptr;
}
