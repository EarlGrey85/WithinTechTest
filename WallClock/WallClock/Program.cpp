#include "Clock.h"
#include <iostream>
#include <conio.h>

#define esc 27

int main()
{
	Clock clock;
	std::string time;

	do 
	{
		printf("enter time:\n");
		std::cin >> time;
		clock.PrintClockHandsAngle(time);
		printf("press any key, for next input\n\n");
	} while (getch() != esc);

	return 0;
}