


#include "HField.h"


namespace Graphics
{
namespace Graphics3D
{
namespace HField
{

float HEIGHTFIELD::GetHeight(VECTOR2 Position)
{
	float Ret=0.0f;
	float r;
	for(unsigned long j=0;j<NoBumps;j++)
	{
		r=Bumps[j].Omega*(Position-Bumps[j].Position).abs();
		if(r<PI)
			Ret+=Bumps[j].Height*cos(r);
	}

	return Ret;
}

void HEIGHTFIELD::NewField(unsigned short Maxx, unsigned short Maxy, float DistX, float DistY, VECTOR2 T, float DistTX, float DistTY, SURFACE *Tex)
{
	New((unsigned long)Maxx*(unsigned long)Maxy*2);

	(*this).Maxx=Maxx;
	(*this).Maxy=Maxy;
	(*this).DistX=DistX;
	(*this).DistY=DistY;
	(*this).T=T;
	(*this).DistTX=DistTX;
	(*this).DistTY=DistTY;

	unsigned long Tol;
	float xp, yp;
	float xt, yt;

	unsigned short x, y;


	yp=0.0f;
	yt=T.y;
	Tol=0;

	for(y=0;y<Maxy;y++, yp+=DistY, yt+=DistTY)
	{
		xp=0.0f;
		xt=T.x;
		for(x=0;x<Maxx;x++, xp+=DistX, xt+=DistTX, Tol+=2)
		{
			Tris[Tol]=	TRILGTEX3D(VECTOR3(xp, yp, 0.0f), VECTOR3(xp+DistX, yp, 0.0f), VECTOR3(xp+DistX, yp+DistY, 0.0f), VECTOR2(xt, yt), VECTOR2(xt+DistTX, yt), VECTOR2(xt+DistTX, yt+DistTY), Tex, VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f));
			Tris[Tol+1]=TRILGTEX3D(VECTOR3(xp, yp, 0.0f), VECTOR3(xp+DistX, yp+DistY, 0.0f), VECTOR3(xp, yp+DistY, 0.0f), VECTOR2(xt, yt), VECTOR2(xt+DistTX, yt+DistTY), VECTOR2(xt, yt+DistTY), Tex, VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f));
		}
	}

	Log.Message("Initialized Height Field.");
}

void HEIGHTFIELD::CreateHeightField(unsigned long NoBumps, float MaxHeight, float MaxRadius)
{
	(*this).NoBumps=NoBumps;
	Heap.Free(Bumps);
	Bumps=(BUMP *)Heap.Allocate(NoBumps, sizeof(BUMP));

	unsigned long i, j;
	float r;
	float alpha;

	for(i=0;i<NoBumps;i++)
	{
		Bumps[i].Position=Random.RVECTOR2(VECTOR2(Maxx*DistX+2*MaxRadius, Maxy*DistY+2*MaxRadius))-VECTOR2(MaxRadius, MaxRadius);
		Bumps[i].Height=MaxHeight*Random.RCurve(0, 1, &_HeightDistributionCurve);
		Bumps[i].Omega=PI/(MaxRadius*Random.RCurve(0, 1, &_RadiusDistributionCurve));
	}

	for(i=0;i<NoTris;i++)
	{
		Tris[i].a.z=0;
		for(j=0;j<NoBumps;j++)
		{
			r=Bumps[j].Omega*(VECTOR2(Tris[i].a.x, Tris[i].a.y)-Bumps[j].Position).abs();

			if(r<PI)
				Tris[i].a.z+=Bumps[j].Height*Cos(r);
		}

		Tris[i].b.z=0;
		for(j=0;j<NoBumps;j++)
		{
			r=Bumps[j].Omega*(VECTOR2(Tris[i].b.x, Tris[i].b.y)-Bumps[j].Position).abs();
			alpha=-Bumps[j].Position.CosAngleWith(VECTOR2(Tris[i].b.x, Tris[i].b.y));

			if(r<PI)
				Tris[i].b.z+=Bumps[j].Height*Cos(r);
		}

		Tris[i].c.z=0;
		for(j=0;j<NoBumps;j++)
		{
			r=Bumps[j].Omega*(VECTOR2(Tris[i].c.x, Tris[i].c.y)-Bumps[j].Position).abs();
			alpha=-Bumps[j].Position.CosAngleWith(VECTOR2(Tris[i].c.x, Tris[i].c.y));

			if(r<PI)
				Tris[i].c.z+=Bumps[j].Height*Cos(r);
		}
	}

	CalculateNormals();

	Log.Message("Generated Random Height Field.");
}

void HEIGHTFIELD::CalculateNormals()
{
	unsigned short x;
	unsigned short y;

	unsigned long i;

	VECTOR3 Normal;
	VECTOR3 *N1;
	VECTOR3 *N2;
	VECTOR3 *N3;
	VECTOR3 *N4;
	VECTOR3 *N5;
	VECTOR3 *N6;
	float PN1;
	float PN2;
	float PN3;
	float PN4;
	float PN5;
	float PN6;

	unsigned long Tol;

	for(i=0;i<NoTris;i++)
	{
		Tris[i].aN=Tris[i].bN=Tris[i].cN=TriNormal(Tris[i].a, Tris[i].b, Tris[i].c);

		Tris[i].aN.Setabs(1);
		Tris[i].bN.Setabs(1);
		Tris[i].cN.Setabs(1);
	}

	// Upper-Left Corner
	Tol=0;
	N1=&Tris[Tol].aN;
	PN1=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

	Tol=1;
	N2=&Tris[Tol].aN;
	PN2=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

	*N1=*N2=(*N1*PN1+*N2*PN2)/(PN1+PN2);
	//*N1=*N2=(*N1+*N2)/2;


	// Lower-Left Corner - Only one normal


	// Upper-Right Corner - Only one normal


	// Lower-Right Corner
	Tol=2*((Maxx-1)+Maxx*(Maxy-1));
	N1=&Tris[Tol].cN;
	PN1=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

	Tol=2*((Maxx-1)+Maxx*(Maxy-1))+1;
	N2=&Tris[Tol].bN;
	PN2=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

	*N1=*N2=(*N1*PN1+*N2*PN2)/(PN1+PN2);
	//*N1=*N2=(*N1+*N2)/2;

	for(x=1;x<Maxx;x++)
	{
		// Upper edge
		Tol=2*(x-1);
		N1=&Tris[Tol].bN;
		PN1=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*x+1;
		N2=&Tris[Tol].aN;
		PN2=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*x;
		N3=&Tris[Tol].aN;
		PN3=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		*N1=*N2=*N3=(*N1*PN1+*N2*PN2+*N3*PN3)/(PN1+PN2+PN3);
		//*N1=*N2=*N3=(*N1+*N2+*N3)/3;


		// Lower edge
		Tol=2*(x-1)+1+2*Maxx*(Maxy-1);
		N1=&Tris[Tol].bN;
		PN1=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*(x-1)+2*Maxx*(Maxy-1);
		N2=&Tris[Tol].cN;
		PN2=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*x+1+2*Maxx*(Maxy-1);
		N3=&Tris[Tol].cN;
		PN3=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		*N1=*N2=*N3=(*N1*PN1+*N2*PN2+*N3*PN3)/(PN1+PN2+PN3);
		//*N1=*N2=*N3=(*N1+*N2+*N3)/3;
	}

	for(y=1;y<Maxy;y++)
	{
		// Left edge
		Tol=2*(y-1)*Maxx+1;
		N1=&Tris[Tol].cN;
		PN1=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*y*Maxx;
		N2=&Tris[Tol].aN;
		PN2=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*y*Maxx+1;
		N3=&Tris[Tol].aN;
		PN3=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		*N1=*N2=*N3=(*N1*PN1+*N2*PN2+*N3*PN3)/(PN1+PN2+PN3);
		//*N1=*N2=*N3=(*N1+*N2+*N3)/3;


		// Right edge
		Tol=2*(y-1)*Maxx+2*(Maxx-1);
		N1=&Tris[Tol].cN;
		PN1=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*(y-1)*Maxx+1+2*(Maxx-1);
		N2=&Tris[Tol].bN;
		PN2=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		Tol=2*y*Maxx+2*(Maxx-1);
		N3=&Tris[Tol].bN;
		PN3=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

		*N1=*N2=*N3=(*N1*PN1+*N2*PN2+*N3*PN3)/(PN1+PN2+PN3);
		//*N1=*N2=*N3=(*N1+*N2+*N3)/3;
	}

	for(y=1;y<Maxy;y++)
		for(x=1;x<Maxx;x++)
		{
			// General
			Tol=2*((x-1)+(y-1)*Maxx)+1;
			N1=&Tris[Tol].bN;
			PN1=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			Tol=2*((x-1)+(y-1)*Maxx);
			N2=&Tris[Tol].cN;
			PN2=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			Tol=2*(x+(y-1)*Maxx)+1;
			N3=&Tris[Tol].cN;
			PN3=1/(Tris[Tol].c-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			Tol=2*(x+y*Maxx);
			N4=&Tris[Tol].aN;
			PN4=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			Tol=2*(x+y*Maxx)+1;
			N5=&Tris[Tol].aN;
			PN5=1/(Tris[Tol].a-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			Tol=2*((x-1)+y*Maxx);
			N6=&Tris[Tol].bN;
			PN6=1/(Tris[Tol].b-TriMassCenter(Tris[Tol].a, Tris[Tol].b, Tris[Tol].c)).abs();

			*N1=*N2=*N3=*N4=*N5=*N6=(*N1*PN1+*N2*PN2+*N3*PN3+*N4*PN4+*N5*PN5+*N6*PN6)/(PN1+PN2+PN3+PN4+PN5+PN6);
			//*N1=*N2=*N3=*N4=*N5=*N6=(*N1+*N2+*N3+*N4+*N5+*N6)/6;
		}

	for(i=0;i<NoTris;i++)
	{
		if(Tris[i].aN==VECTOR3(0.0f, 0.0f, 0.0f))
			Tris[i].aN=VECTOR3(0.0f, 0.0f, 1.0f);
		else
			Tris[i].aN.Setabs(1.0f);

		if(Tris[i].bN==VECTOR3(0.0f, 0.0f, 0.0f))
			Tris[i].bN=VECTOR3(0.0f, 0.0f, 1.0f);
		else
			Tris[i].bN.Setabs(1.0f);

		if(Tris[i].cN==VECTOR3(0.0f, 0.0f, 0.0f))
			Tris[i].cN=VECTOR3(0.0f, 0.0f, 1.0f);
		else
			Tris[i].cN.Setabs(1.0f);
	}
}

void HEIGHTFIELD::DisplaceMap(SURFACE *Map, float MinH, float MaxH)
{
	unsigned long i;

	for(i=0;i<NoTris;i++)
	{
		Tris[i].a.z=MinH+(MaxH-MinH)*(float)(Map->GetPixelCheck(Tris[i].at.x, Tris[i].at.y).g)/255.0f;
		Tris[i].b.z=MinH+(MaxH-MinH)*(float)(Map->GetPixelCheck(Tris[i].bt.x, Tris[i].bt.y).g)/255.0f;
		Tris[i].c.z=MinH+(MaxH-MinH)*(float)(Map->GetPixelCheck(Tris[i].ct.x, Tris[i].ct.y).g)/255.0f;
	}


	CalculateNormals();

	Log.Message("Created HeightMap from Bitmap.");
}

void HEIGHTFIELD::DisplaceMap(char MapFile[], float MinH, float MaxH)
{
	SURFACE Map;
	Map.LoadFromBMP(MapFile);

	DisplaceMap(&Map, MinH, MaxH);

	Map.Close();
}


} // namespace HField
} // namespace Graphics3D
} // namespace Graphics
