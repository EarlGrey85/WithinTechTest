#include "Clock.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <ctype.h>

Clock::Clock()
{
}


Clock::~Clock()
{
}

inline bool IsDecimalNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();

	while (it != s.end() && isdigit(*it))
	{
		++it;
	}

	return !s.empty() && it == s.end();
}

void Clock::PrintClockHandsAngle(const std::string time)
{
	std::string delimeter = ":";
	int pos = time.find(delimeter);
	std::string hoursStr = time.substr(0, pos);
	std::string minutesStr = time.substr(pos + 1);

	if (!IsDecimalNumber(hoursStr) || !IsDecimalNumber(minutesStr))
	{
		std::printf("non-digital values, check input\n\n");
		return;
	}

	int hours = std::stoi(hoursStr);
	int minutes = std::stoi(minutesStr);

	if (!CheckTimeFormat(hours, minutes))
	{
		std::printf("wrong time format\n\n");
		return;
	}

	float clockHandsAngle = GetClockHandsAngle(hours, minutes);
	printf("Angle between clock hands is %f\n", clockHandsAngle);
}

bool Clock::CheckTimeFormat(int &hours, int &minutes)
{
	

	if ((hours < 0 || hours > 24 || minutes < 0 || minutes > 60) || (hours == 24 && minutes > 0))
	{
		return false;
	}

	return true;
}

float Clock::GetClockHandsAngle(const int &hours, const int &minutes)
{
	auto hourHandAngle = hours * HOUR_HAND_ANGLE_PER_HOUR + minutes * HOUR_ANGLE_PER_MINUTE; 
	auto minuteHandAngle = minutes * MINUTE_ANGLE_PER_MINUTE;
	
	hourHandAngle = fmod(hourHandAngle, 360);   // for 24-hour notation.. to not overlap 360 deg
	auto handAngle = abs(hourHandAngle - minuteHandAngle);

	return handAngle > 180 ? handAngle = 360 - handAngle : handAngle;
}
