#include "JSON.h"
#include <iostream>
#include <string>
#include <sstream>

JSON::JSON()
{
}


JSON::~JSON()
{
}

void JSON::Visualize_data(const std::string &data)
{
	std::ostringstream sStream;
	int depth = 0;

	for (int i = 0; i < data.length(); ++i)
	{
		auto ch = &data[i];

		if (*ch == '}' || *ch == ']')
		{
			--depth;
			ShiftString(sStream, depth);
		}

		if (*ch == '{' || *ch == '[')
		{
			ShiftString(sStream, depth);
		}

		sStream << *ch;

		if (*ch == ',')
		{
			ShiftString(sStream, depth);
		}

		if (*ch == '{' || *ch == '[')
		{
			depth++;
			ShiftString(sStream, depth);
		}
	}

	std::cout << sStream.str() << std::endl;
}
 
inline std::string JSON::GetIndent(const int &depth)
{
	return std::string(depth, ' ');
}

inline std::string JSON::ShiftString(std::ostringstream &sStream, const int &depth)
{
	sStream << "\n" << GetIndent(depth);
	return sStream.str();
}
