


#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_


#include "Geometry.h"
#include "3DGraph.h"


using namespace Geometry;
using namespace Graphics::Graphics2D;


namespace Graphics
{
namespace Graphics3D
{


struct VIEWPORT
{
	float FOV;
	float SecondaryAngle;
	float NearPlane;
	float sNearPlane;
	float FarPlane;

	float AspectRatio;
	float M3x;
	float M3y;

	float R32; // Divide between space M3 and screen Max

	VECTOR3 Position;
	VECTOR3 Direction;
	VECTOR3 Up;

	// Call these if FOV has been modified
	void CalculateM3()
	{
		(*this).M3x=(*this).NearPlane*2.0f*Tan((*this).FOV/2.0f);
		(*this).M3y=M3x/AspectRatio;
	
		(*this).R32=(*this).M3x/Graph.Target->Maxx;
	}
	
	void CalculatesNearPlane()
	{
		sNearPlane=1.0f/NearPlane;
	}
			
	void CalculateSecondaryAngle()
	{
		SecondaryAngle=2.0f*Atan(M3y/(2.0f*(*this).NearPlane));
	}
	
	VIEWPORT(const float FOV, const float NearPlane, const float FarPlane, const float AspectRatio, const VECTOR3 &Position, const VECTOR3 &Direction, const VECTOR3 &Up)
	{
		(*this).FOV=FOV;
		(*this).NearPlane=NearPlane;
		(*this).FarPlane=FarPlane;
		(*this).AspectRatio=AspectRatio;
		(*this).Position=Position;
		(*this).Direction=Direction;
		(*this).Up=Up;
	
		(*this).CalculateM3();
		(*this).CalculateSecondaryAngle();
		(*this).CalculatesNearPlane();
	}
	
	VIEWPORT()
	{
	}
	
	~VIEWPORT()
	{
	}
};


struct TRANSFORMATION
{
	VECTOR3 Translate;
	float RotateZ;
	float RotateY;
	float RotateU;

	VECTOR3 Transform(VECTOR3 Vector)
	{
		Vector+=(*this).Translate;
		Vector.RotateZ((*this).RotateZ);
		Vector.RotateY((*this).RotateY);
		Vector.RotateZ((*this).RotateU);
	
		return Vector;
	}
	
	void GetTransformation(VIEWPORT ViewPort)
	{
		(*this).Translate=-ViewPort.Position;
		// no need: ViewPort.Position=0;
	
		if(ViewPort.Direction.x!=0.0f||ViewPort.Direction.y!=0.0f)
		{
			(*this).RotateZ=-Acos(ViewPort.Direction.x/Sqrt(ViewPort.Direction.x*ViewPort.Direction.x+ViewPort.Direction.y*ViewPort.Direction.y));
			if(ViewPort.Direction.y<0.0f)
				(*this).RotateZ=-PI2-(*this).RotateZ;
		}
		else
			(*this).RotateZ=0.0f;
		ViewPort.Direction.RotateZ((*this).RotateZ);
		ViewPort.Up.RotateZ((*this).RotateZ);
	
		if(ViewPort.Direction.x!=0.0f||ViewPort.Direction.z!=0.0f)
		{
			(*this).RotateY=-Acos(ViewPort.Direction.z/Sqrt(ViewPort.Direction.x*ViewPort.Direction.x+ViewPort.Direction.z*ViewPort.Direction.z));
			if(ViewPort.Direction.x<0.0f)
				(*this).RotateY=-PI2-(*this).RotateY;
		}
		else
			(*this).RotateY=0.0f;
		// no need: ViewPort.Direction.RotateY((*this).RotateY);
		ViewPort.Up.RotateY((*this).RotateY);
	
		if(ViewPort.Up.x!=0.0f||ViewPort.Up.y!=0.0f)
		{
			(*this).RotateU=Acos(ViewPort.Up.x/Sqrt(ViewPort.Up.x*ViewPort.Up.x+ViewPort.Up.y*ViewPort.Up.y));
			if(ViewPort.Up.y<0.0f)
				(*this).RotateU=PI2-(*this).RotateU;
			(*this).RotateU=PIs2-(*this).RotateU;
		}
		else
			(*this).RotateU=0.0f;
		// no need: ViewPort.Direction.RotateZ((*this).RotateU);
		// no need: ViewPort.Up.RotateZ((*this).RotateU);
	}
};


} // namespace Graphics3D
} // namespace Graphics


#endif // #ifndef _VIEWPORT_H_
