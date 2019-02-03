


#ifndef _3DLINE_H_
#define _3DLINE_H_


#include "Color.h"
#include "Geometry.h"
#include "3DGraph.h"
#include "ViewPort.h"


using namespace Graphics::Graphics2D;


namespace Graphics
{
namespace Graphics3D
{
namespace Line3D
{


struct LINE3D
{
public:
	VECTOR3 a;
	VECTOR3 b;
	RGB Color;

	LINE3D()
	{
	}
	
	LINE3D(VECTOR3 a, VECTOR3 b, RGB Color)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).Color=Color;
	}
	
	~LINE3D()
	{
	}

	void Project()
	{
		VECTOR3 aTransformed, bTransformed;
		BOOL aNotActive;
		BOOL bNotActive;
		float recip;
		float a, b, c, d;
		float x, y;

		VECTOR2 ap, bp;
		
		aTransformed=Graph3D.Project((*this).a);
		bTransformed=Graph3D.Project((*this).b);

		aNotActive=ZCheckNP(aTransformed.z);
		bNotActive=ZCheckNP(bTransformed.z);
	
		if(aNotActive&&bNotActive)
			return;
	
		if(!aNotActive&&!bNotActive)
		{
			ap=Graph3D.ToScreenCoords(aTransformed);
			bp=Graph3D.ToScreenCoords(bTransformed);
		}
		else
		{
			// Get Line equation: y=az+b, x=cz+d
			recip=1/(aTransformed.z-bTransformed.z);
			a=(aTransformed.y-bTransformed.y)*recip;
			b=(aTransformed.z*bTransformed.y-bTransformed.z*aTransformed.y)*recip;
			c=(aTransformed.x-bTransformed.x)*recip;
			d=(aTransformed.z*bTransformed.x-bTransformed.z*aTransformed.x)*recip;
		
			// Get intersection point with NearPlane
			x=c*Graph3D.ViewPort.NearPlane+d;
			y=a*Graph3D.ViewPort.NearPlane+b;
		
			// Transform to screen coords
			x=Graph.Target->Maxx*(x+Graph3D.ViewPort.M3x/2)/Graph3D.ViewPort.M3x;
			y=Graph.Target->Maxy*(Graph3D.ViewPort.M3y/2-y)/Graph3D.ViewPort.M3y;
		
			if(bNotActive)
			{
				ap=Graph3D.ToScreenCoords(aTransformed);
				bp=VECTOR2(x, y);

				bTransformed.z=Graph3D.ViewPort.NearPlane;
				bNotActive=0;
			}
			else // if(aNotActive)
			{
				ap=VECTOR2(x, y);
				bp=Graph3D.ToScreenCoords(bTransformed);

				aTransformed.z=Graph3D.ViewPort.NearPlane;
				aNotActive=0;
			}
		}
	

		// Get Line equation	
		if(ap.x!=bp.x) // y=ax+b
		{
			recip=1/(ap.x-bp.x);
			a=(ap.y-bp.y)*recip;
			b=(ap.x*bp.y-ap.y*bp.x)*recip;
	
			if(ap.x<0)
				if(bp.x<0)
					return;
				else
				{
					if(aTransformed.z!=bTransformed.z)
						aTransformed.z-=ap.x*(bp.x-ap.x)/(bTransformed.z-aTransformed.z);
					ap=VECTOR2(0, b);
				}
			else if(ap.x>=(float)Graph.Target->Maxx)
				if(bp.x>=(float)Graph.Target->Maxx)
					return;
				else
				{
					if(aTransformed.z!=bTransformed.z)
						aTransformed.z+=(Graph.Target->Maxx-1-ap.y)*(bp.y-ap.y+1)/(bTransformed.z-aTransformed.z);
					ap=VECTOR2(Graph.Target->Maxx-1, a*(Graph.Target->Maxx-1)+b);
				}
		}
	
		// Get Line equation
		if(ap.y!=bp.y) // x=ay+b
		{
			recip=1/(ap.y-bp.y);
			a=(ap.x-bp.x)*recip;
			b=(ap.y*bp.x-ap.x*bp.y)*recip;
	
			if(ap.y<0)
				if(bp.y<0)
					return;
				else
				{
					if(aTransformed.z!=bTransformed.z)
						aTransformed.z-=ap.y*(bp.y-ap.y)/(bTransformed.z-aTransformed.z);
					ap=VECTOR2(b, 0);
				}
			else if(ap.y>=Graph.Target->Maxy)
				if(bp.y>=Graph.Target->Maxy)
					return;
				else
				{
					if(aTransformed.z!=bTransformed.z)
						aTransformed.z+=(Graph.Target->Maxx-1-ap.x)*(bp.x-ap.x+1)/(bTransformed.z-aTransformed.z);
					ap=VECTOR2(a*(Graph.Target->Maxy-1)+b, Graph.Target->Maxy-1);
				}
		}
	
		if(ap==bp)
			if(ap.x<0||ap.x>=Graph.Target->Maxx||ap.y<0||ap.y>=Graph.Target->Maxy)
				return;
	
		_Draw((PIXEL)ap, (PIXEL)bp, aTransformed.z, bTransformed.z);
	}


private:
	
	void _Draw(PIXEL ap, PIXEL bp, float az, float bz);
};


struct LINE_MESH
{
	unsigned int NoLines;
	LINE3D *Lines;

	LINE_MESH()
	{
		Lines=NULL;
		NoLines=0;
	}
	
	~LINE_MESH()
	{
		Delete();
	}
	
	void New(unsigned int NoLines)
	{
		Delete();
		Lines=(LINE3D *)Heap.PAllocate(NoLines, sizeof(LINE3D));
		(*this).NoLines=NoLines;
	}
	
	void Delete()
	{
		if(Lines!=NULL)
		{
			Heap.Free(Lines);
			Lines=NULL;
		}
		NoLines=0;
	}

	void Project();

	void LoadMesh(char Filename[]);
	void LoadMeshB(char Filename[]);
	void SaveMesh(char Filename[]);
	void SaveMeshB(char Filename[]);

	void Scale(float abs);
	void RotateX(float alpha);
	void RotateY(float alpha);
	void RotateZ(float alpha);
	void Rotate(float alpha, VECTOR3 Axis);
	void Translate(VECTOR3 Translation);
	void ChangeEntireColor(RGB Color);

	void VertexBlend(void (*BlendingFunction)(LINE3D &Line));

	void CopyFrom(LINE_MESH &Src);
};


} // namespace Line3D
} // namespace Graphics3D
} // namespace Graphics


#endif // #ifndef _3DLINE_H_