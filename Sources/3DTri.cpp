


#include "3DTri.h"


using namespace Graphics::Graphics2D;


namespace Graphics
{
namespace Graphics3D
{
namespace Triangle3D
{


namespace _Private
{


// struct _TRI3D

template <class VALUES, class CONSTS> void _TRI3D <VALUES, CONSTS> ::Project()
{
	PACKET3D Original;

	PACKET2D Unsorted;
	PACKET2D Sorted;

	PACKET3D Copy1;
	PACKET3D Copy2;
	PACKET3D Copy3;

	// Points active
	BOOL aA;
	BOOL bA;
	BOOL cA;

	I_TYPE a, b, c, d;
	I_TYPE recip;


	Original.a.Transformed=Graph3D.Project((*this).a);
	Original.b.Transformed=Graph3D.Project((*this).b);
	Original.c.Transformed=Graph3D.Project((*this).c);

	if(Graph3D.ClipFP(Original.a.Transformed, Original.b.Transformed, Original.c.Transformed))
		return;

	StartValuesProject(Original);

	aA=ZCheckNP(Original.a.Transformed.z);
	bA=ZCheckNP(Original.b.Transformed.z);
	cA=ZCheckNP(Original.c.Transformed.z);

	switch(aA+bA+cA)
	{
	case(0):
		{
			// Entering 2D layer
			// Transform to screen coords
			Unsorted.a.Position2D=Graph3D.ToScreenCoords(Original.a.Transformed);
			Unsorted.b.Position2D=Graph3D.ToScreenCoords(Original.b.Transformed);
			Unsorted.c.Position2D=Graph3D.ToScreenCoords(Original.c.Transformed);

			// Frostrum Clipping
			if(Graph3D.Clip3V(Unsorted.a.Position2D, Unsorted.b.Position2D, Unsorted.c.Position2D))
				return;

			// Back face Culling
			if(Graph3D.Cull(Unsorted.a.Position2D, Unsorted.b.Position2D, Unsorted.c.Position2D))
				return;

			// Copy values for interpolation
			Unsorted.CopyValuesFrom(Original);

			// Calculate Constants
			CalculateConsts(Sorted, Original);

			// Sort points by y
			_Sort(Sorted, Unsorted);
			// Draw triangle
			_Draw(Sorted);

			break;
		}

	case(1):
		{
			Copy1=Original;

			if(bA)
			{
				Copy1.b=Original.a;					
				Copy1.a=Original.b;
			}
			else if(cA)
			{
				Copy1.c=Original.a;					
				Copy1.a=Original.c;
			}

			Copy2=Copy1;

			// Get Line equation (AB): y=az+b, x=cz+d
			recip=1/(Copy2.a.Transformed.z-Copy2.b.Transformed.z);
			a=(Copy2.a.Transformed.y-Copy2.b.Transformed.y)*recip;
			b=(Copy2.a.Transformed.z*Copy2.b.Transformed.y-Copy2.b.Transformed.z*Copy2.a.Transformed.y)*recip;
			c=(Copy2.a.Transformed.x-Copy2.b.Transformed.x)*recip;
			d=(Copy2.a.Transformed.z*Copy2.b.Transformed.x-Copy2.b.Transformed.z*Copy2.a.Transformed.x)*recip;

			// Get intersection point with NearPlane
			Copy2.a.Transformed.x=c*(I_TYPE)Graph3D.ViewPort.NearPlane+d;
			Copy2.a.Transformed.y=a*(I_TYPE)Graph3D.ViewPort.NearPlane+b;
			Copy2.a.Transformed.z=(I_TYPE)Graph3D.ViewPort.NearPlane;

			// Entering 2D layer
			// Transform to screen coords
			Unsorted.a.Position2D=Graph3D.ToScreenCoords(Copy2.a.Transformed);
			Unsorted.b.Position2D=Graph3D.ToScreenCoords(Copy2.b.Transformed);
			Unsorted.c.Position2D=Graph3D.ToScreenCoords(Copy2.c.Transformed);

			// Frostrum Clipping
			if(Graph3D.Clip3V(Unsorted.a.Position2D, Unsorted.b.Position2D, Unsorted.c.Position2D))
				return;

			// Calculate new interpolation values in A
			Copy2.a.Values=Copy2.b.Values-(Copy2.b.Values-Copy2.a.Values)*(Copy2.a.Transformed.z-Copy2.b.Transformed.z)*recip;

			// Copy values for interpolation
			Unsorted.CopyValuesFrom(Copy2);

			// Calculate Constants
			CalculateConsts(Sorted, Copy2);

			// Sort points by y
			_Sort(Sorted, Unsorted);
			// Draw triangle
			_Draw(Sorted);


			// Cycle the points
			Copy2.c=Copy2.a;
			Copy2.a=Copy1.a;
			Copy2.b=Copy1.c;

			Copy3=Copy2;

			// Get Line equation (AC): y=az+b, x=cz+d
			recip=1/(Copy3.a.Transformed.z-Copy3.b.Transformed.z);
			a=(Copy3.a.Transformed.y-Copy3.b.Transformed.y)*recip;
			b=(Copy3.a.Transformed.z*Copy3.b.Transformed.y-Copy3.b.Transformed.z*Copy3.a.Transformed.y)*recip;
			c=(Copy3.a.Transformed.x-Copy3.b.Transformed.x)*recip;
			d=(Copy3.a.Transformed.z*Copy3.b.Transformed.x-Copy3.b.Transformed.z*Copy3.a.Transformed.x)*recip;

			// Get intersection point with NearPlane
			Copy3.a.Transformed.x=c*(I_TYPE)Graph3D.ViewPort.NearPlane+d;
			Copy3.a.Transformed.y=a*(I_TYPE)Graph3D.ViewPort.NearPlane+b;
			Copy3.a.Transformed.z=(I_TYPE)Graph3D.ViewPort.NearPlane;

			// Entering 2D layer
			// Transform to screen coords
			Unsorted.a.Position2D.x=(I_TYPE)Graph.Target->Maxx*((I_TYPE)Copy3.a.Transformed.x+(I_TYPE)Graph3D.ViewPort.M3x/(I_TYPE)2)/(I_TYPE)Graph3D.ViewPort.M3x;
			Unsorted.a.Position2D.y=(I_TYPE)Graph.Target->Maxy*((I_TYPE)Graph3D.ViewPort.M3y/(I_TYPE)2-(I_TYPE)Copy3.a.Transformed.y)/(I_TYPE)Graph3D.ViewPort.M3y;

			Unsorted.b.Position2D=Graph3D.ToScreenCoords(Copy3.b.Transformed);
			Unsorted.c.Position2D=Graph3D.ToScreenCoords(Copy3.c.Transformed);

			// Frostrum Clipping
			if(Graph3D.Clip3V(Unsorted.a.Position2D, Unsorted.b.Position2D, Unsorted.c.Position2D))
				return;

			// Calculate new interpolation values in A
			Copy3.a.Values=Copy3.b.Values-(Copy3.b.Values-Copy3.a.Values)*(Copy3.a.Transformed.z-Copy3.b.Transformed.z)*recip;

			// Copy values for interpolation
			Unsorted.CopyValuesFrom(Copy3);

			// Calculate Constants
			CalculateConsts(Sorted, Copy3);

			// Sort points by y
			_Sort(Sorted, Unsorted);
			// Draw triangle
			_Draw(Sorted);

			break;
		}

	case(2):
		{
			Copy1=Original;

			if(!aA)
			{
				Copy1.b=Original.a;
				Copy1.a=Original.b;
			}
			else if(!cA)
			{
				Copy1.c=Original.b;
				Copy1.b=Original.c;
			}

			Copy2=Copy1;

			// Get Line equation (AB): y=az+b, x=cz+d
			recip=1/(Copy2.a.Transformed.z-Copy2.b.Transformed.z);
			a=(Copy2.a.Transformed.y-Copy2.b.Transformed.y)*recip;
			b=(Copy2.a.Transformed.z*Copy2.b.Transformed.y-Copy2.b.Transformed.z*Copy2.a.Transformed.y)*recip;
			c=(Copy2.a.Transformed.x-Copy2.b.Transformed.x)*recip;
			d=(Copy2.a.Transformed.z*Copy2.b.Transformed.x-Copy2.b.Transformed.z*Copy2.a.Transformed.x)*recip;

			// Get intersection point with NearPlane
			Copy2.a.Transformed.x=c*(I_TYPE)Graph3D.ViewPort.NearPlane+d;
			Copy2.a.Transformed.y=a*(I_TYPE)Graph3D.ViewPort.NearPlane+b;
			Copy2.a.Transformed.z=(I_TYPE)Graph3D.ViewPort.NearPlane;

			// Entering 2D layer
			// Transform to screen coords
			Unsorted.c.Position2D.x=(I_TYPE)Graph.Target->Maxx*((I_TYPE)Copy2.a.Transformed.x+(I_TYPE)Graph3D.ViewPort.M3x/(I_TYPE)2)/(I_TYPE)Graph3D.ViewPort.M3x;
			Unsorted.c.Position2D.y=(I_TYPE)Graph.Target->Maxy*((I_TYPE)Graph3D.ViewPort.M3y/(I_TYPE)2-(I_TYPE)Copy2.a.Transformed.y)/(I_TYPE)Graph3D.ViewPort.M3y;

			// Calculate new interpolation values in A
			Copy2.a.Values=Copy2.b.Values-(Copy2.b.Values-Copy2.a.Values)*(Copy2.a.Transformed.z-Copy2.b.Transformed.z)*recip;


			// Cycle the points
			Copy2.c=Copy2.a;
			Copy2.a=Copy1.c;

			Copy3=Copy2;

			// Get Line equation (AC): y=az+b, x=cz+d
			recip=1/(Copy3.a.Transformed.z-Copy3.b.Transformed.z);
			a=(Copy3.a.Transformed.y-Copy3.b.Transformed.y)*recip;
			b=(Copy3.a.Transformed.z*Copy3.b.Transformed.y-Copy3.b.Transformed.z*Copy3.a.Transformed.y)*recip;
			c=(Copy3.a.Transformed.x-Copy3.b.Transformed.x)*recip;
			d=(Copy3.a.Transformed.z*Copy3.b.Transformed.x-Copy3.b.Transformed.z*Copy3.a.Transformed.x)*recip;

			// Get intersection point with NearPlane
			Copy3.a.Transformed.x=c*(I_TYPE)Graph3D.ViewPort.NearPlane+d;
			Copy3.a.Transformed.y=a*(I_TYPE)Graph3D.ViewPort.NearPlane+b;
			Copy3.a.Transformed.z=(I_TYPE)Graph3D.ViewPort.NearPlane;

			// Entering 2D layer
			// Transform to screen coords
			Unsorted.a.Position2D.x=(I_TYPE)Graph.Target->Maxx*((I_TYPE)Copy3.a.Transformed.x+(I_TYPE)Graph3D.ViewPort.M3x/(I_TYPE)2)/(I_TYPE)Graph3D.ViewPort.M3x;
			Unsorted.a.Position2D.y=(I_TYPE)Graph.Target->Maxy*((I_TYPE)Graph3D.ViewPort.M3y/(I_TYPE)2-(I_TYPE)Copy3.a.Transformed.y)/(I_TYPE)Graph3D.ViewPort.M3y;

			Unsorted.b.Position2D=Graph3D.ToScreenCoords(Copy3.b.Transformed);

			// Frostrum Clipping
			if(Graph3D.Clip3V(Unsorted.a.Position2D, Unsorted.b.Position2D, Unsorted.c.Position2D))
				return;

			// Calculate new interpolation values in A
			Copy3.a.Values=Copy3.b.Values-(Copy3.b.Values-Copy3.a.Values)*(Copy3.a.Transformed.z-Copy3.b.Transformed.z)*recip;

			// Copy values for interpolation
			Unsorted.CopyValuesFrom(Copy3);

			// Calculate Constants
			CalculateConsts(Sorted, Copy3);

			// Sort points by y
			_Sort(Sorted, Unsorted);
			// Draw triangle
			_Draw(Sorted);

			break;
		}
	}
}

template <class VALUES, class CONSTS> void _TRI3D <VALUES, CONSTS> ::_Draw(PACKET2D &Sorted)
{	
	I_TYPE Sx, Ex;
	register short y;
	VALUES Sv, Ev;

	StartValuesDraw(Sorted);

#define Ax Sorted.a.Position2D.x
#define Bx Sorted.b.Position2D.x
#define Cx Sorted.c.Position2D.x

#define Av Sorted.a.Values
#define Bv Sorted.b.Values
#define Cv Sorted.c.Values

	short Ay=(short)Sorted.a.Position2D.y;
	short By=(short)Sorted.b.Position2D.y;
	short Cy=(short)Sorted.c.Position2D.y;

	VALUES dv1, dv2, dv3;
	I_TYPE dx1, dx2, dx3;

	I_TYPE recip;

#ifdef VERTEX_COUNTER
	Graph3D.VertCount+=3;
#endif // #ifdef VERTEX_COUNTER

	if(By>Ay)
	{
		recip=1/(I_TYPE)(By-Ay);
		
		dx1=(Bx-Ax)*recip;
		dv1=(Bv-Av)*recip;
	}
	else
	{
		dx1=0.0f;
		dv1.Zero();
	}
		
	if(Cy>Ay)
	{
		recip=1/(I_TYPE)(Cy-Ay);
		
		dx2=(Cx-Ax)*recip;
		dv2=(Cv-Av)*recip;
	}
	else
	{
		dx2=0.0f;
		dv2.Zero();
	}
		
	if(Cy>By)
	{
		recip=1/(I_TYPE)(Cy-By);
		
		dx3=(Cx-Bx)*recip;
		dv3=(Cv-Bv)*recip;
	}
	else
	{
		dx3=0.0f;
		dv3.Zero();
	}

	Sx=Ex=Ax;
	y=Ay;
	Sv=Ev=Av;
	
	if(dx1?(dx1>dx2):(dx2>dx3))
	{
		if(y<0.0f)
			if(By>=0.0f)
			{
				Sx-=dx2*(I_TYPE)y;
				Ex-=dx1*(I_TYPE)y;

				Sv-=dv2*(I_TYPE)y;
				Ev-=dv1*(I_TYPE)y;

				y=0;
			}
			else if(y<By)
			{
				y=y-By;

				Sx-=dx2*(I_TYPE)y;
				Ex-=dx1*(I_TYPE)y;

				Sv-=dv2*(I_TYPE)y;
				Ev-=dv1*(I_TYPE)y;

				y=By;
			}

		if(By>Graph.Target->Maxy)
			By=Graph.Target->Maxy;

		for(; y<By; y++, Sx+=dx2, Ex+=dx1, Sv+=dv2, Ev+=dv1)
			_Line((short)Sx, (short)Ex, y, Sv, Ev, Sorted.Con);

		if(y>=Graph.Target->Maxy)
				return;

		Ex=Bx;
		Ev=Bv;

		if(y<0)
			if(Cy>=0)
			{
				Sx-=dx2*(I_TYPE)y;
				Ex-=dx3*(I_TYPE)y;

				Sv-=dv2*(I_TYPE)y;
				Ev-=dv3*(I_TYPE)y;

				y=0;
			}
			else if(y<Cy)
			{
				y=y-Cy;

				Sx-=dx2*(I_TYPE)y;
				Ex-=dx3*(I_TYPE)y;

				Sv-=dv2*(I_TYPE)y;
				Ev-=dv3*(I_TYPE)y;

				y=Cy;
			}

		if(Cy>Graph.Target->Maxy)
			Cy=Graph.Target->Maxy;

		for(; y<Cy; y++, Sx+=dx2, Ex+=dx3, Sv+=dv2, Ev+=dv3)
			_Line((short)Sx, (short)Ex, y, Sv, Ev, Sorted.Con);
	}
	else
	{
		if(y<0)
			if(By>=0)
			{
				Sx-=dx1*(I_TYPE)y;
				Ex-=dx2*(I_TYPE)y;

				Sv-=dv1*(I_TYPE)y;
				Ev-=dv2*(I_TYPE)y;

				y=0;
			}
			else if(y<By)
			{
				y=y-By;

				Sx-=dx1*(I_TYPE)y;
				Ex-=dx2*(I_TYPE)y;

				Sv-=dv1*(I_TYPE)y;
				Ev-=dv2*(I_TYPE)y;

				y=By;
			}

		if(By>Graph.Target->Maxy)
			By=Graph.Target->Maxy;


		for(; y<By; y++, Sx+=dx1, Ex+=dx2, Sv+=dv1, Ev+=dv2)
			_Line((short)Sx, (short)Ex, y, Sv, Ev, Sorted.Con);

		if(y>=Graph.Target->Maxy)
				return;

		Sx=Bx;
		Sv=Bv;

		if(y<0)
			if(Cy>=0)
			{
				Sx-=dx3*(I_TYPE)y;
				Ex-=dx2*(I_TYPE)y;

				Sv-=dv3*(I_TYPE)y;
				Ev-=dv2*(I_TYPE)y;

				y=0;
			}
			else if(y<Cy)
			{
				y=y-Cy;

				Sx-=dx3*(I_TYPE)y;
				Ex-=dx2*(I_TYPE)y;

				Sv-=dv3*(I_TYPE)y;
				Ev-=dv2*(I_TYPE)y;

				y=Cy;
			}

		if(Cy>Graph.Target->Maxy)
			Cy=Graph.Target->Maxy;

		for(; y<Cy; y++, Sx+=dx3, Ex+=dx2, Sv+=dv3, Ev+=dv2)
			_Line((short)Sx, (short)Ex, y, Sv, Ev, Sorted.Con);
	}

#undef Ax
#undef Bx
#undef Cx

#undef Av
#undef Bv
#undef Cv
}

template <class VALUES, class CONSTS> void _TRI3D <VALUES, CONSTS> ::_Line(short x1, short x2, short y, VALUES v1, VALUES v2, const CONSTS &Con)
{
	if(x1>=Graph.Target->Maxx||x2<0)
		return;
	if(x2<=x1)
		return;

	VALUES dv=(v2-v1)/(I_TYPE)(x2-x1);

	if(x1<0)
	{
		v1-=dv*x1;
		x1=0;
	}
	
	if(x2>Graph.Target->Maxx)
		x2=Graph.Target->Maxx;

	register unsigned long i=Graph.Target->tol((unsigned short)x1, (unsigned short)y, Graph.Target->Maxx);
	register unsigned long MaxTol=i+(unsigned long)x2-(unsigned long)x1;

	for(; i<MaxTol; i++, v1+=dv)
	{
		ExtractValues(v2, v1);
		HandlePixel(i, v2, Con);
	}
}

template <class VALUES, class CONSTS> void _TRI3D <VALUES, CONSTS> ::_Sort(PACKET2D &Sorted, PACKET2D &Obj)
{
	if(Obj.a.Position2D.y<Obj.b.Position2D.y)
		if(Obj.a.Position2D.y<Obj.c.Position2D.y)
		{
			// A x x
			Sorted.a=Obj.a;
			
			if(Obj.b.Position2D.y>Obj.c.Position2D.y)
			{
				// A C B
				Sorted.b=Obj.c;
				Sorted.c=Obj.b;
			}
			else
			{
				// A B C
				Sorted.b=Obj.b;
				Sorted.c=Obj.c;
			}
		}
		else
		{
			// C A B
			Sorted.a=Obj.c;
			Sorted.b=Obj.a;
			Sorted.c=Obj.b;
		}
	else
		if(Obj.b.Position2D.y<Obj.c.Position2D.y)
		{
			// B x x
			Sorted.a=Obj.b;
			
			if(Obj.a.Position2D.y<Obj.c.Position2D.y)
			{
				// B A C
				Sorted.b=Obj.a;
				Sorted.c=Obj.c;
			}
			else
			{
				// B C A
				Sorted.b=Obj.c;
				Sorted.c=Obj.a;
			}
		}
		else
		{
			// C B A
			Sorted.a=Obj.c;
			Sorted.b=Obj.b;
			Sorted.c=Obj.a;
		}			
}


} // namespace _Private


namespace _Private
{


// struct _TRI_MESH

template <class TRI> void _TRI_MESH <TRI> ::TriTransform(void(*TransformingFunction)(TRI &Tri))
{
	for(unsigned long i=0;i<NoTris;i++)
		TransformingFunction(Tris[i]);
}

template <class TRI> void _TRI_MESH <TRI> ::Translate(const VECTOR3 &Translation)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].a+=Translation;
		Tris[i].b+=Translation;
		Tris[i].c+=Translation;
	}
}

template <class TRI> void _TRI_MESH <TRI> ::RotateX(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].a.RotateX(Alpha);
		Tris[i].b.RotateX(Alpha);
		Tris[i].c.RotateX(Alpha);
	}
}

template <class TRI> void _TRI_MESH <TRI> ::RotateY(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].a.RotateY(Alpha);
		Tris[i].b.RotateY(Alpha);
		Tris[i].c.RotateY(Alpha);
	}
}

template <class TRI> void _TRI_MESH <TRI> ::RotateZ(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].a.RotateZ(Alpha);
		Tris[i].b.RotateZ(Alpha);
		Tris[i].c.RotateZ(Alpha);
	}
}

template <class TRI> void _TRI_MESH <TRI> ::Scale(const float Coef)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].a*=Coef;
		Tris[i].b*=Coef;
		Tris[i].c*=Coef;
	}
}


} // namespace _Private


namespace CustomMeshes
{


// struct TRIC_MESH

void TRIC_MESH::LoadMesh(char Filename[])
{
	Delete();

	FILE *File=fopen(Filename, "rb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	fread(&NoTris, sizeof(unsigned long), 1, File);
	New(NoTris);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fread(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fread(&Tris[i].ac, sizeof(RGB), 1, File);
		fread(&Tris[i].bc, sizeof(RGB), 1, File);
		fread(&Tris[i].cc, sizeof(RGB), 1, File);
	}

	fclose(File);

	Log.Message("Loaded Triangle Mesh from file ", Filename);
}

void TRIC_MESH::SaveMesh(char Filename[])
{
	FILE *File=fopen(Filename, "wb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening output file ", Filename);
		exit(1);
	}

	fwrite(&NoTris, sizeof(unsigned long), 1, File);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fwrite(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fwrite(&Tris[i].ac, sizeof(RGB), 1, File);
		fwrite(&Tris[i].bc, sizeof(RGB), 1, File);
		fwrite(&Tris[i].cc, sizeof(RGB), 1, File);
	}

	fclose(File);

	Log.Message("Saved Triangle Mesh to file ", Filename);
}


// struct TRITEX_MESH

void TRITEX_MESH::LoadMesh(char Filename[])
{
	Delete();

	FILE *File=fopen(Filename, "rb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	fread(&NoTris, sizeof(unsigned long), 1, File);
	New(NoTris);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fread(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fread(&Tris[i].at, sizeof(PIXEL), 1, File);
		fread(&Tris[i].bt, sizeof(PIXEL), 1, File);
		fread(&Tris[i].ct, sizeof(PIXEL), 1, File);

		Tris[i].Tex=NULL;
	}

	fclose(File);

	Log.Message("Loaded Triangle Mesh from file ", Filename);
}

void TRITEX_MESH::SaveMesh(char Filename[])
{
	FILE *File=fopen(Filename, "wb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening output file ", Filename);
		exit(1);
	}

	fwrite(&NoTris, sizeof(unsigned long), 1, File);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fwrite(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fwrite(&Tris[i].at, sizeof(PIXEL), 1, File);
		fwrite(&Tris[i].bt, sizeof(PIXEL), 1, File);
		fwrite(&Tris[i].ct, sizeof(PIXEL), 1, File);
	}

	fclose(File);

	Log.Message("Saved Triangle Mesh to file ", Filename);
}

void TRITEX_MESH::ChangeTexture(SURFACE *Tex)
{
	for(unsigned long i=0;i<NoTris;i++)
		Tris[i].Tex=Tex;
}


// struct TRILGTEX_MESH

void TRILGTEX_MESH::LoadMesh(char Filename[])
{
	Delete();

	FILE *File=fopen(Filename, "rb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	fread(&NoTris, sizeof(unsigned long), 1, File);
	New(NoTris);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fread(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fread(&Tris[i].at, sizeof(PIXEL), 1, File);
		fread(&Tris[i].bt, sizeof(PIXEL), 1, File);
		fread(&Tris[i].ct, sizeof(PIXEL), 1, File);

		Tris[i].Tex=NULL;

		fread(&Tris[i].aN, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].bN, sizeof(VECTOR3), 1, File);
		fread(&Tris[i].cN, sizeof(VECTOR3), 1, File);
	}

	fclose(File);

	Log.Message("Loaded Triangle Mesh from file ", Filename);
}

void TRILGTEX_MESH::SaveMesh(char Filename[])
{
	FILE *File=fopen(Filename, "wb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening output file ", Filename);
		exit(1);
	}

	fwrite(&NoTris, sizeof(unsigned long), 1, File);

	for(unsigned long i=0;i<NoTris;i++)
	{
		fwrite(&Tris[i].a, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].b, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].c, sizeof(VECTOR3), 1, File);

		fwrite(&Tris[i].at, sizeof(PIXEL), 1, File);
		fwrite(&Tris[i].bt, sizeof(PIXEL), 1, File);
		fwrite(&Tris[i].ct, sizeof(PIXEL), 1, File);

		fwrite(&Tris[i].aN, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].bN, sizeof(VECTOR3), 1, File);
		fwrite(&Tris[i].cN, sizeof(VECTOR3), 1, File);
	}

	fclose(File);

	Log.Message("Saved Triangle Mesh to file ", Filename);
}

void TRILGTEX_MESH::RotateX(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].aN+=Tris[i].a;
		Tris[i].bN+=Tris[i].b;
		Tris[i].cN+=Tris[i].c;

		Tris[i].aN.RotateX(Alpha);
		Tris[i].bN.RotateX(Alpha);
		Tris[i].cN.RotateX(Alpha);

		Tris[i].a.RotateX(Alpha);
		Tris[i].b.RotateX(Alpha);
		Tris[i].c.RotateX(Alpha);

		Tris[i].aN-=Tris[i].a;
		Tris[i].bN-=Tris[i].b;
		Tris[i].cN-=Tris[i].c;
	}
}

void TRILGTEX_MESH::RotateY(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].aN+=Tris[i].a;
		Tris[i].bN+=Tris[i].b;
		Tris[i].cN+=Tris[i].c;

		Tris[i].aN.RotateY(Alpha);
		Tris[i].bN.RotateY(Alpha);
		Tris[i].cN.RotateY(Alpha);

		Tris[i].a.RotateY(Alpha);
		Tris[i].b.RotateY(Alpha);
		Tris[i].c.RotateY(Alpha);

		Tris[i].aN-=Tris[i].a;
		Tris[i].bN-=Tris[i].b;
		Tris[i].cN-=Tris[i].c;
	}
}

void TRILGTEX_MESH::RotateZ(const float Alpha)
{
	for(unsigned long i=0;i<NoTris;i++)
	{
		Tris[i].aN+=Tris[i].a;
		Tris[i].bN+=Tris[i].b;
		Tris[i].cN+=Tris[i].c;

		Tris[i].aN.RotateZ(Alpha);
		Tris[i].bN.RotateZ(Alpha);
		Tris[i].cN.RotateZ(Alpha);

		Tris[i].a.RotateZ(Alpha);
		Tris[i].b.RotateZ(Alpha);
		Tris[i].c.RotateZ(Alpha);

		Tris[i].aN-=Tris[i].a;
		Tris[i].bN-=Tris[i].b;
		Tris[i].cN-=Tris[i].c;
	}
}

void TRILGTEX_MESH::ChangeTexture(SURFACE *Tex)
{
	for(unsigned long i=0;i<NoTris;i++)
		Tris[i].Tex=Tex;
}


} // namespace CustomMeshes


} // namespace Triangle3D
} // namespace Graphics3D
} // namespace Graphics
