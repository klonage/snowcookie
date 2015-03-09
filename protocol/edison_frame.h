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

	static std::shared_ptr<EdisonFrame> parse_frame (char* buffer, int size);

	Type get_type () const { return frame_type; }

	virtual int serialize (char* data) const = 0;

	static int pack_and_serialize (const EdisonFrame& frame, char* data);
};

class GetStatusEdisonFrame : public EdisonFrame
{
	bool request;
	long long size = 0;
	int log_count = 0;
public:
	virtual ~GetStatusEdisonFrame () {}
	GetStatusEdisonFrame (bool request = true) : EdisonFrame (GET_STATUS), request (request) {}
	GetStatusEdisonFrame (char* buffer, int size);

	void set_data (int log_count);

	int serialize (char* data) const override;

	static constexpr int max_size = 3 + sizeof (long long) + sizeof (int);

	long long get_size () const { return size; }
	int get_log_count () const { return log_count; }
};

}

#endif /* EDISON_FRAME_H_ */
