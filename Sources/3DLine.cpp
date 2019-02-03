


#include "3DLine.h"


namespace Graphics
{
namespace Graphics3D
{
namespace Line3D
{


// class LINE3D

void LINE3D::_Draw(PIXEL ap, PIXEL bp, float az, float bz)
{
	short i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
	float dz, pz;

#ifdef VERTEX_COUNTER
	Graph3D.VertCount+=2;
#endif // #ifdef VERTEX_COUNTER

	dx=bp.x-ap.x;
	dy=bp.y-ap.y;
	dz=bz-az;
	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=Sgn(dx);
	sdy=Sgn(dy);
	x=dyabs>>1;
	y=dxabs>>1;
	px=ap.x;
	py=ap.y;
	pz=az;

	Graph3D.PutPixelZCheck(px, py, pz, Color);

	if (dxabs>=dyabs)
	{
		if(dxabs==0&&dyabs==0)
			return;
		
		dz/=dyabs;
		
		for(i=0;i<dxabs;i++)
		{
			y+=dyabs;
			
			if (y>=dxabs)
			{
				y-=dxabs;
				py+=sdy;
				pz+=dz;
			}
			px+=sdx;
			Graph3D.PutPixelZCheck(px, py, pz, Color);
		}
	}
	else
	{
		dz/=dxabs;
		
		for(i=0;i<dyabs;i++)
		{
			x+=dxabs;
			
			if (x>=dyabs)
			{
				x-=dyabs;
				px+=sdx;
				pz+=dz;
			}
			py+=sdy;
			Graph3D.PutPixelZCheck(px, py, pz, Color);
		}
	}
}


// class LINE_MESH

void LINE_MESH::Project()
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
		(*this).Lines[i].Project();
}

void LINE_MESH::LoadMesh(char Filename[]) // Text
{
	fstream gin(Filename, ios::in);

	unsigned int aux;
	uint8  r, g, b;

	gin>>aux;
	(*this).New(aux);

	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		gin>>(*this).Lines[i].a.x>>(*this).Lines[i].a.y>>(*this).Lines[i].a.z;
		gin>>(*this).Lines[i].b.x>>(*this).Lines[i].b.y>>(*this).Lines[i].b.z;
		gin>>aux; //a=(uint8)aux; // Reserved for blending
		gin>>aux; r=(uint8)aux;
		gin>>aux; g=(uint8)aux;
		gin>>aux; b=(uint8)aux;

		(*this).Lines[i].Color=RGB(r, g, b);
	}

	gin.close();

	Log.Message("Loaded Line Mesh from file ", Filename);
}

void LINE_MESH::LoadMeshB(char Filename[]) // Binary
{
	FILE *File=fopen(Filename, "rb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	fread(&NoLines, sizeof(unsigned int), 1, File);
	New(NoLines);
	fread(Lines, sizeof(LINE3D), NoLines, File);

	fclose(File);

	Log.Message("Loaded Line Mesh from file ", Filename);
}

void LINE_MESH::SaveMesh(char Filename[]) // Text
{
	fstream gout(Filename, ios::out);

	gout<<NoLines<<endl<<endl<<endl;

	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		gout<<(*this).Lines[i].a.x<<' '<<(*this).Lines[i].a.y<<' '<<(*this).Lines[i].a.z<<endl;
		gout<<(*this).Lines[i].b.x<<' '<<(*this).Lines[i].b.y<<' '<<(*this).Lines[i].b.z<<endl;
		gout<<255 // (unsigned)(*this).Lines[i].color.a // Reserved for blending
			 <<' '<<(unsigned)(*this).Lines[i].Color.r
			 <<' '<<(unsigned)(*this).Lines[i].Color.g
			 <<' '<<(unsigned)(*this).Lines[i].Color.b
			 <<endl;
		gout<<endl;
	}

	gout.close();

	Log.Message("Saved Line Mesh to file ", Filename);
}

void LINE_MESH::SaveMeshB(char Filename[]) // Binary
{
	FILE *File=fopen(Filename, "wb");

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening output file ", Filename);
		exit(1);
	}

	fwrite(&NoLines, sizeof(unsigned int), 1, File);
	fwrite(Lines, sizeof(LINE3D), NoLines, File);

	fclose(File);

	Log.Message("Saved Line Mesh to file ", Filename);
}

void LINE_MESH::Scale(float abs)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a.Scale(abs);
		(*this).Lines[i].b.Scale(abs);
	}
}

void LINE_MESH::RotateX(float alpha)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a.RotateX(alpha);
		(*this).Lines[i].b.RotateX(alpha);
	}
}

void LINE_MESH::RotateY(float alpha)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a.RotateY(alpha);
		(*this).Lines[i].b.RotateY(alpha);
	}
}

void LINE_MESH::RotateZ(float alpha)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a.RotateZ(alpha);
		(*this).Lines[i].b.RotateZ(alpha);
	}
}

void LINE_MESH::Rotate(float alpha, VECTOR3 Axis)
{
	if(Axis.x==0&&Axis.y==0&&Axis.z==0)
		return;

	float AngleY;
	float AngleZ;

	if(Axis.x==0&&Axis.y==0)
		AngleZ=0;
	else
	{
		AngleZ=-Acos(Axis.x/Sqrt(Axis.x*Axis.x+Axis.y*Axis.y));
		if(Axis.y<0)
			AngleZ=-AngleZ;
	}
	Axis.RotateZ(AngleZ);

	if(Axis.x==0&&Axis.z==0)
		AngleY=0;
	else
	{
		AngleY=-Acos(Axis.x/Sqrt(Axis.x*Axis.x+Axis.z*Axis.z));
		if(Axis.z<0)
			AngleY=-AngleY;
	}
	// no need: Axis.RotateY(AngleY);



	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a.RotateZ(AngleZ);
		(*this).Lines[i].a.RotateY(AngleY);

		(*this).Lines[i].a.RotateX(alpha);

		(*this).Lines[i].a.RotateY(-AngleY);
		(*this).Lines[i].a.RotateZ(-AngleZ);


		(*this).Lines[i].b.RotateZ(AngleZ);
		(*this).Lines[i].b.RotateY(AngleY);

		(*this).Lines[i].b.RotateX(alpha);

		(*this).Lines[i].b.RotateY(-AngleY);
		(*this).Lines[i].b.RotateZ(-AngleZ);
	}
}

void LINE_MESH::Translate(VECTOR3 Translation)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
	{
		(*this).Lines[i].a=(*this).Lines[i].a+Translation;
		(*this).Lines[i].b=(*this).Lines[i].b+Translation;
	}
}

void LINE_MESH::ChangeEntireColor(RGB Color)
{
	for(unsigned int i=0;i<(*this).NoLines;i++)
		(*this).Lines[i].Color=Color;
}

void LINE_MESH::VertexBlend(void(*BlendingFunction)(LINE3D &Line))
{
	for(unsigned int i=0;i<NoLines;i++)
		BlendingFunction(Lines[i]);
}

void LINE_MESH::CopyFrom(LINE_MESH &Src)
{
	New(Src.NoLines);

	for(unsigned int i=0;i<Src.NoLines;i++)
		(*this).Lines[i]=Src.Lines[i];
}


} // namespace Line3D
} // namespace Graphics3D
} // namespace Graphics
