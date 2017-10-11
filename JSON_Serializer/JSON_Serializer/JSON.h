#include <string>

#pragma once
class JSON
{
public:
	JSON();
	~JSON();
	static void Visualize_data(const std::string& data);

private:
	static inline std::string GetIndent(const int &depth);
	static inline std::string ShiftString(std::ostringstream &sStream, const int &depth);
};

