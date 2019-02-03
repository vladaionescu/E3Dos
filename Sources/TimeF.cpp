


#include "TimeF.h"


namespace TimeFunctions
{


// class dTIME

void dTIME::Reset()
{
	Start=Time();
	Last=Time();
	Variation=1.0f;
	ActVariation=0.0f;
	ActStart=Time();
	Calls=0;
	TCalls=0;
	Elapsed=0.0f;
	TElapsed=0.0f;
}

void dTIME::Next()
{
	Calls++;
	TCalls++;

	if(Elapsed>7.0f&&TElapsed>100.0f)
	{
		Calls=1;
		Elapsed=0.0f;
		Variation=ActVariation;
		ActStart=Time();
	}

	Elapsed+=Variation;
	TElapsed+=Variation;
	ActVariation=(float)(Time()-ActStart)/(float)Calls;
	Last=Time();
}

float dTIME::FPS()
{
	if(Variation!=0.0f)
		return 1.0f/Variation*ClockTick;

	return 0;
}

float dTIME::AvgFPS()
{
	if(Last!=Start)
		return (float)TCalls/(float)(Last-Start)*ClockTick;

	return 0.0f;
}


} // namespace TimeFunctions
