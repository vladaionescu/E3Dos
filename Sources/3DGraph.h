



#ifndef _3DGRAPH_H_
#define _3DGRAPH_H_


// Special switches

#define LightHDR
//#define SpecularLighting
//#define ABuffering
#define VERTEX_COUNTER


#include "2DGraph.h"

#include "Viewport.h"
#include "Lighting.h"
#include "ABuffer.h"


using namespace Geometry;
using namespace Graphics::Graphics2D;
using namespace Graphics::Graphics3D::Lighting;


namespace Graphics
{
namespace Graphics3D
{


class GRAPH3D
{
public:

	static LIGHTMANAGER LightManager;
	static VIEWPORT ViewPort;
	static TRANSFORMATION Transformation;
	static float *ZBuffer;
	static AlphaBuffering::_A_NODE **ABuffer;

#ifdef VERTEX_COUNTER
	static unsigned long VertCount;
#endif // #ifdef VERTEX_COUNTER


	GRAPH3D()
	{
		ZBuffer=NULL;
		ABuffer=NULL;
		SetStMethods();
	}
	
	~GRAPH3D()
	{
		Close();
	}
	
	void Initialize();	
	void Close();

	static void ClearZBuffer()
	{
		memset(ZBuffer, 127, Graph.Target->MaxT<<2);
	}

	static void DoABuffer();
	static void SetABufferNULL();


#ifdef ZCheckNP
#undef ZCheckNP
#endif // #ifdef ZCheckNP

#ifdef ZCheckCP
#undef ZCheckCP
#endif // #ifdef ZCheckCP

#ifdef ZCheck
#undef ZCheck
#endif // #ifdef ZCheck

#define ZCheckNP(z) (z<=ViewPort.NearPlane)
#define ZCheckCP(Tol, z) (ZBuffer[Tol]<=z)
#define ZCheck(Tol, z) ZCheckNP(z)||ZCheckCP(Tol, z)


	void PutPixelZCheck(const short x, const short y, const float z, const RGB Color)
	{
		if(Graph.Target->IsOut(x, y))
			return;

		if(ZCheckNP(z))
			return;

		unsigned long Tol=Graph.Target->tol(x, y, Graph.Target->Maxx);

#ifndef ABuffering
		if(ZCheckCP(Tol, z))
			return;
#endif // #ifndef ABuffering

		PutPixelZ(Tol, z, Color);
	}

	void PutPixelZ(const short x, const short y, const float z, const RGB Color)
	{
		if(ZCheckNP(z))
			return;

		unsigned long Tol=Graph.Target->tol(x, y, Graph.Target->Maxx);

#ifndef ABuffering
		if(ZCheckCP(Tol, z))
			return;
#endif // #ifndef ABuffering

		PutPixelZ(Tol, z, Color);
	}

	void PutPixelZCheck(const short x, const short y, const float z, const ARGB Color)
	{
		if(Graph.Target->IsOut(x, y))
			return;

		if(ZCheckNP(z))
			return;

		unsigned long Tol=Graph.Target->tol(x, y, Graph.Target->Maxx);

#ifndef ABuffering
		if(ZCheckCP(Tol, z))
			return;
#endif // #ifndef ABuffering

		PutPixelZ(Tol, z, Color);
	}

	void PutPixelZ(const short x, const short y, const float z, const ARGB Color)
	{
		if(ZCheckNP(z))
			return;

		unsigned long Tol=Graph.Target->tol(x, y, Graph.Target->Maxx);

#ifndef ABuffering
		if(ZCheckCP(Tol, z))
			return;
#endif // #ifndef ABuffering

		PutPixelZ(Tol, z, Color);
	}

	void PutPixelZ(const unsigned long Tol, const float z, const RGB Color)
	{
#ifdef ABuffering
		AlphaBuffering::AddNode(&ABuffer[Tol], AlphaBuffering::_A_INFO(ARGB(255, Color), z));
#else // #ifdef ABuffering
		ZBuffer[Tol]=z;
		Graph.Target->PutPixel(Tol, Color);
#endif // #else // #ifdef ABuffering
	}

	void PutPixelZ(const unsigned long Tol, const float z, const ARGB Color)
	{
#ifdef ABuffering
		AlphaBuffering::AddNode(&ABuffer[Tol], AlphaBuffering::_A_INFO(Color, z));
#else // #ifdef ABuffering
		EmptyFunc(z); // (Avoid a warning); ZBuffer[Tol]=z;
		Graph.Target->PutPixel(Tol, Color);
#endif // #else // #ifdef ABuffering
	}

	void SetStMethods()
	{
		Clip3V=&St_Clip3V;
		ClipFP=&St_ClipFP;
		Cull=&St_Cull;
		Next=&St_Next;
	}

	BOOL (* Clip3V)(const VECTOR2 &Position2Da, const VECTOR2 &Position2Db, const VECTOR2 &Position2Dc); // Returns 1 if the tri is not to be drawn
	BOOL (* ClipFP)(const VECTOR3 &Transformeda, const VECTOR3 &Transformedb, const VECTOR3 &Transformedc); // Returns 1 if the tri is not to be drawn
	BOOL (* Cull)(const VECTOR2 &Position2Da, const VECTOR2 &Position2Db, const VECTOR2 &Position2Dc); // Returns 1 if the tri is not to be drawn
	void (* Next)();

	VECTOR3 Project(const VECTOR3 &Point)
	{
		return Transformation.Transform(Point);
	}

	VECTOR2 ToScreenCoords(const VECTOR3 &TransformedPoint)
	{
		return VECTOR2(	(float)Graph.Target->Maxx*((float)(TransformedPoint.x*ViewPort.NearPlane/TransformedPoint.z)+ViewPort.M3x/2)/ViewPort.M3x,
						(float)Graph.Target->Maxy*(ViewPort.M3y/2-(float)(TransformedPoint.y*ViewPort.NearPlane/TransformedPoint.z))/ViewPort.M3y
		);
	}

	static BOOL St_Clip3V(const VECTOR2 &Position2Da, const VECTOR2 &Position2Db, const VECTOR2 &Position2Dc)
	{
		return
			(Position2Da.y<0&&Position2Db.y<0&&Position2Dc.y<0)||
			(Position2Da.y>=Graph.Target->Maxy&&Position2Db.y>=Graph.Target->Maxy&&Position2Dc.y>=Graph.Target->Maxy)||
            (Position2Da.x<0&&Position2Db.x<0&&Position2Dc.x<0)||
            (Position2Da.x>=Graph.Target->Maxx&&Position2Db.x>=Graph.Target->Maxx&&Position2Dc.x>=Graph.Target->Maxx);
	}

	// Known lmitation: This doesn't work very well if there are tris in the scene that are at high distance from the camera
	static BOOL St_NoClip3V(const VECTOR2 &, const VECTOR2 &, const VECTOR2 &)
	{
		return 0;
	}

	static BOOL St_ClipFP(const VECTOR3 &Transformeda, const VECTOR3 &Transformedb, const VECTOR3 &Transformedc)
	{
		if(Transformeda.z>ViewPort.FarPlane&&Transformedb.z>ViewPort.FarPlane&&Transformedc.z>ViewPort.FarPlane)
			return 1;

		return 0;
	}

	static BOOL St_NoClipFP(const VECTOR3 &, const VECTOR3 &, const VECTOR3 &)
	{
		return 0;
	}

	// FIXME: Not working 100%
	static BOOL St_Cull(const VECTOR2 &Position2Da, const VECTOR2 &Position2Db, const VECTOR2 &Position2Dc)
	{
		VECTOR2 v1=Position2Dc-Position2Db;
		VECTOR2 v2=Position2Da-Position2Db;

		float a=(Position2Db-Position2Dc).abs();
		float b=(Position2Dc-Position2Da).abs();
		float c=(Position2Da-Position2Db).abs();
		float p=(a+b+c)/2;

#define S p*(p-a)*(p-b)*(p-c)
		return (S>100.0f&&v1.x*v2.y-v1.y*v2.x<0);
#undef S

		/*
		VECTOR2 v3=Position2Da-Position2Dc;
		VECTOR2 v4=Position2Db-Position2Dc;

		VECTOR2 v5=Position2Db-Position2Da;
		VECTOR2 v6=Position2Dc-Position2Da;

#define TYPE long double
		TYPE v1x=(TYPE)Position2Dc.x-(TYPE)Position2Db.x;
		TYPE v1y=(TYPE)Position2Dc.y-(TYPE)Position2Db.y;

		TYPE v2x=(TYPE)Position2Da.x-(TYPE)Position2Db.x;
		TYPE v2y=(TYPE)Position2Da.y-(TYPE)Position2Db.y;

		TYPE v3x=(TYPE)Position2Da.x-(TYPE)Position2Dc.x;
		TYPE v3y=(TYPE)Position2Da.y-(TYPE)Position2Dc.y;

		TYPE v4x=(TYPE)Position2Db.x-(TYPE)Position2Dc.x;
		TYPE v4y=(TYPE)Position2Db.y-(TYPE)Position2Dc.y;

		TYPE v5x=(TYPE)Position2Db.x-(TYPE)Position2Da.x;
		TYPE v5y=(TYPE)Position2Db.y-(TYPE)Position2Da.y;

		TYPE v6x=(TYPE)Position2Dc.x-(TYPE)Position2Da.x;
		TYPE v6y=(TYPE)Position2Dc.y-(TYPE)Position2Da.y;
#undef TYPE

		return ((v1x*v2y-v1y*v2x<0)&&(v3x*v4y-v3y*v4x<0)&&(v5x*v6y-v5y*v6x<0));
		*/

		/*
#define a (long double)Position2Da
#define b (long double)Position2Db
#define c (long double)Position2Dc

		return ((b.x-a.x)*(a.y-c.y)-(b.y-a.y)*(a.x-c.x))>0;

#undef a
#undef b
#undef c
		*/
	}

	static BOOL St_NoCull(const VECTOR2 &, const VECTOR2 &, const VECTOR2 &)
	{
		return 0;
	}

	static void St_Next()
	{
#ifndef ABuffering
		ClearZBuffer();
#endif // #ifndef ABuffering

		Transformation.GetTransformation(ViewPort);
		ViewPort.CalculateM3();
		ViewPort.CalculateSecondaryAngle();
		ViewPort.CalculatesNearPlane();

#ifdef VERTEX_COUNTER
		VertCount=0;
#endif // #ifdef VERTEX_COUNTER

#ifdef PIXEL_COUNTER
		PixelCount=0;
#endif // #ifdef PIXEL_COUNTER
	}
};

extern GRAPH3D Graph3D;


#undef ZCheckNP
#undef ZCheckCP
#undef ZCheck


#define ZCheckNP(z) (z<=Graph3D.ViewPort.NearPlane)

#ifdef ABuffering
#define ZCheckCP(Tol, z) 0
#else // #ifdef ABuffering
#define ZCheckCP(Tol, z) (Graph3D.ZBuffer[Tol]<=z)
#endif // #else // #ifdef ABuffering

#define ZCheck(Tol, z) ZCheckNP(z)||ZCheckCP(Tol, z)


} // namespace Graphics3D
} // namespace Graphics


#include "3DLine.h"
#include "3DTri.h"


#endif // #ifndef _3DGRAPH_H_
