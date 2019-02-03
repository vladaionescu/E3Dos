


#ifndef _RANDOM_H_
#define _RANDOM_H_


#include <stdlib.h>

#include "TimeF.h"
#include "Geometry.h"
#include "2DGraph.h"


using namespace TimeFunctions;
using namespace Geometry;
using namespace Graphics::Graphics2D;


namespace RandomGenerator
{


class RANDOM
{
public:

	RANDOM()
	{
		Restart();
	}

	~RANDOM()
	{
	}

	void Restart()
	{
		srand(time(NULL));
		delay(1);
		srand(Time()+time(NULL));
	}

	unsigned long Rulong()
	{
		return ((unsigned long)(uint8)(rand()+rand())<<24)|((unsigned long)(uint8)(rand()+rand())<<16)|((unsigned long)(uint8)(rand()+rand())<<8)|((unsigned long)(uint8)(rand()+rand())&0xff);
	}

	unsigned long Rulong(unsigned long a, unsigned long b) // An unsigned long value in [a, b]
	{
		if(a>b)
			return Rulong(b, a);

		return (Rulong()%(b-a+1))+a;
	}

	long Rlong() // FIXME: Not good at very big numbers
	{
		return (long)Rulong();
	}

	long Rlong(long a, long b) // A long value in [a, b]
	{
		if(a>b)
			return Rlong(b, a);

		a+=0x7fffffff;
		b+=0x7fffffff;

		return ((Rulong()%(b-a+1))+a)-0x7fffffff;
	}

	float Rfloat() // A float value in [0, 1]
	{
		return (float)Rlong(0, 65536)/65536.;
	}

	float Rfloat(float a, float b) // Not to be used with big numbers // A float value in [a, b]
	{
		long FPa, FPb; // Fixed points

		FPa=a*65536.0f;
		FPb=b*65536.0f;

		return (float)Rlong(FPa, FPb)/65536.0f;
	}

	float RCurve(float x1, float x2, float (* CurveFunction)(float x)) // Distribution curve based
	{
		return CurveFunction(Rfloat(x1, x2));
	}

	VECTOR2 RVECTOR2(VECTOR2 Max)
	{
		return VECTOR2(Rfloat(0, Max.x), Rfloat(0, Max.y));
	}

	RGB RRGB()
	{
		return (RGB)(Rulong()&0x00ffffff);
	}
};

extern RANDOM Random;


} // namespace RandomGenerator

#endif // #ifndef _RANDOM_H_