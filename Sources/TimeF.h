



#ifndef _TIMEF_H_
#define _TIMEF_H_


#include "General.h"

#include <time.h>


namespace TimeFunctions
{


#define Time() *(unsigned long *)(0x0000046C)			// Points to the 18.2 Hz System Clock
#define ClockTick 18.2f									// Equivalent to CLK_TCK
#define sClockTick 0.054945054945054945054945054945055f	// 1/ClockTick


class dTIME // used for calculating the variation of time in motion
{
public:
	unsigned long Last;
	unsigned long Start;
	float Variation;
	float ActVariation;
	float TElapsed;
	float Elapsed;
	unsigned long ActStart;
	unsigned long Calls;
	unsigned long TCalls;

	dTIME()
	{
		Reset();
	}
	
	~dTIME()
	{
	}
	
	void Reset();
	void Next();	
	float FPS();
	float AvgFPS();
};


} // namespace TimeFunctions


#endif // ifndef _TIMEF_H_
