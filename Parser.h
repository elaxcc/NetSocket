#ifndef PARSER_H_
#define PARSER_H_

#include "defines.h"

#include <vector>

namespace Net
{

class parser
{
public:
	parser();
	void reset();
	void flush();
	void parse(char *data, int data_len);
	bool is_complete();
	bool got_next_packet();
	HEADER get_header() const;
	std::vector<char> get_data() const;
	static std::vector<char> serialize(
		const HEADER& head, const std::vector<char>& data);

private:
	bool got_header_;
	bool packet_complete_;
	HEADER header_;
	std::vector<char> buffer_;

};

} // Net

#endif /* PARSER_H_ */
