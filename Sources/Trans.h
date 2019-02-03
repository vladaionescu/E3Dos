


#ifndef _TRANS_H_
#define _TRANS_H_


#include "2DGraph.h"


namespace Graphics
{
namespace Graphics2D
{
namespace TransitionEffects
{


class FADE
{
public:

	SURFACE *From;
	SURFACE *To;
	RGB ColorFrom;
	RGB ColorTo;

	void Start(SURFACE *From, SURFACE *To)
	{
		(*this).From=From;
		(*this).To=To;
	}

	void Start(SURFACE *From, RGB To)
	{
		(*this).From=From;
		(*this).To=NULL;

		(*this).ColorTo=To;
	}

	void Start(RGB From, SURFACE *To)
	{
		(*this).From=NULL;
		(*this).To=To;

		(*this).ColorFrom=From;
	}

	void Start(RGB From, RGB To)
	{
		(*this).From=NULL;
		(*this).To=NULL;

		(*this).ColorFrom=From;
		(*this).ColorTo=To;
	}

	void DrawFrame(float Step);
};


class T_EFFECT_1
{
public:

	short RayDelta;
	fRGB RayColor;
	float Ns; // Specular Material

	SURFACE *From;
	SURFACE *To;

	void Start(SURFACE *From, SURFACE *To, short RayDelta=100, fRGB RayColor=fRGB(White), float Ns=5.0f)
	{
		(*this).From=From;
		(*this).To=To;
		(*this).RayDelta=RayDelta;
		(*this).RayColor=RayColor;
		(*this).Ns=Ns;
	}

	void DrawFrame(float Step);
};


} // namespace TransitionEffects
} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _TRANS_H_
