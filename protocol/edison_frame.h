/*
 * edison_frame.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef EDISON_FRAME_H_
#define EDISON_FRAME_H_

#include <memory>

namespace SnowCookie {


class EdisonFrame
{
public:
	enum Type
	{
		GET_STATUS = 0,
		CLEAR_LOGS,
		START_LOG,
		STOP_LOG,
		REBOOT,
		SET_DATA_OUT_DIVISOR
	};

protected:
	Type frame_type;
	bool err_flag = false;

public:
	EdisonFrame (Type type) : frame_type (type) {}
	virtual ~EdisonFrame () {}

	static std::shared_ptr<EdisonFrame> parse_frame (unsigned char* buffer, int size);

	Type get_type () const { return frame_type; }

	virtual int serialize (unsigned char* data);
};

class GetStatusEdisonFrame : public EdisonFrame
{
	bool request;
	long long size = 0;
public:
	virtual ~GetStatusEdisonFrame () {}
	GetStatusEdisonFrame (bool request = true) : EdisonFrame (GET_STATUS), request (request) {}
	GetStatusEdisonFrame (unsigned char* buffer, int size);

	void set_data (int log_count);

	int serialize (unsigned char* data) override;

	static constexpr int max_size = 3 + sizeof (long long);

	long long get_size () const { return size; }
};

}

#endif /* EDISON_FRAME_H_ */
