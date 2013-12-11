#include "Parser.h"

#include <string.h>

namespace Net
{

parser::parser()
	: got_header_(false)
	, packet_complete_(false)
{
}

void parser::reset()
{
	got_header_ = false;
	packet_complete_ = false;

	memset(&header_, 0, sizeof(header_));
	buffer_.clear();
}

void parser::flush()
{
	got_header_ = false;
	packet_complete_ = false;

	buffer_.erase(buffer_.begin(), buffer_.begin() + header_.data_len);
	memset(&header_, 0, sizeof(header_));
}

void parser::parse(char *data, int data_len)
{
	buffer_.insert(buffer_.end(), data, data + data_len);

	if (!packet_complete_)
	{
		if (!got_header_)
		{
			if (buffer_.size() >= sizeof(HEADER))
			{
				memcpy(&header_, buffer_.data(), sizeof(HEADER));
				// delete HEADER structure data from incoming buffer
				buffer_.erase(buffer_.begin(), buffer_.begin() + sizeof(HEADER));
				got_header_ = true;
			}
			else
			{
				return;
			}
		}

		if (buffer_.size() >= header_.data_len)
		{
			packet_complete_ = true;
		}
	}
}

bool parser::is_complete()
{
	return packet_complete_;
}

bool parser::got_next_packet()
{
	if (packet_complete_)
	{
		return buffer_.size() > header_.data_len;
	}
	return !buffer_.empty();
}

HEADER parser::get_header() const
{
	if (!packet_complete_)
	{
		HEADER zero_header;
		memset(&zero_header, 0, sizeof(HEADER));
		return zero_header;
	}
	return header_;
}

std::vector<char> parser::get_data() const
{
	if (packet_complete_)
	{
		return std::vector<char>(
			buffer_.begin(), buffer_.begin() + header_.data_len);
	}
	return std::vector<char>();
}

std::vector<char> parser::serialize(
	const HEADER& head, const std::vector<char>& data)
{
	std::vector<char> out_data;
	char head_buf[sizeof(HEADER)];

	memcpy(head_buf, &head, sizeof(HEADER));

	out_data.insert(out_data.end(), head_buf, head_buf + sizeof(HEADER));
	out_data.insert(out_data.end(), data.begin(), data.end());

	return out_data;
}

} // Net
