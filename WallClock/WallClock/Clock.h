#pragma once
#include <string>
#include <vector>

class Clock
{
	const int HOUR_HAND_ANGLE_PER_HOUR = 30; // 360/12
	const int MINUTE_ANGLE_PER_MINUTE = 6;  // 360/60
	const float HOUR_ANGLE_PER_MINUTE = 0.5;  // 0.5 = 30/60

public:
	
	Clock();
	~Clock();
	void PrintClockHandsAngle(const std::string time);

private:
	bool CheckTimeFormat(int &hours, int &minutes);
	float GetClockHandsAngle(const int &hours, const int &minutes);
};
