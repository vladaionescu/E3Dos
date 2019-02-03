


#ifndef _HFIELD_H_
#define _HFIELD_H_


#include "3DTri.h"
#include "Random.h"


using namespace Graphics::Graphics3D::Triangle3D;
using namespace RandomGenerator;


namespace Graphics
{
namespace Graphics3D
{
namespace HField
{


struct HEIGHTFIELD : TRILGTEX_MESH
{
	struct BUMP
	{
		VECTOR2 Position;
		float Height;
		float Omega;
	};

	unsigned short Maxx;
	unsigned short Maxy;

	float DistX;
	float DistY;
	
	VECTOR2 T;

	float DistTX;
	float DistTY;

	unsigned long NoBumps;
	BUMP *Bumps;

	HEIGHTFIELD()
	{
		Tris=NULL;
		NoTris=0;
		Bumps=NULL;
		NoBumps=0;
	}

	~HEIGHTFIELD()
	{
		Close();
	}

	float GetHeight(VECTOR2 Position);

	void NewField(unsigned short Maxx, unsigned short Maxy, float DistX, float DistY, VECTOR2 T, float DistTX, float DistTY, SURFACE *Tex);
	void CreateHeightField(unsigned long NoBumps, float MaxHeight, float MaxRadius);

	void Close()
	{
		BOOL SendMsg=(Bumps!=NULL);

		Heap.Free(Bumps);
		Delete();

		if(SendMsg)
			Log.Message("Closed Height Field.");
	}

	void CalculateNormals();

	void DisplaceMap(SURFACE *Map, float MinH, float MaxH);
	void DisplaceMap(char MapFile[], float MinH, float MaxH);

private:
	static float _HeightDistributionCurve(float x)
	{
		x*=2.34342045039470f;
		x+=2.26079632679490f;
		return (Tan(x)/x+1.0f)/3.0f;
	}

	static float _RadiusDistributionCurve(float x)
	{
		return (Pow(2.71f, x)-x-0.5f)/1.21f;
	}
};


} // namespace HField
} // namespace Graphics3D
} // namespace Graphics


#endif // #ifndef _HFIELD_H_
