


#include "Surface.h"


namespace Graphics
{
namespace Graphics2D
{


// struct MIPMAP

void MIPMAP::Initialize(BOOL Tiled, BOOL NonMean, unsigned short Maxx, unsigned short Maxy, RGB *ImData)
{
	Close();
	(*this).Tiled=Tiled;
	
	unsigned short i;
	float dx;
	float dy;
	unsigned short l=Max(Maxx, Maxy);

	float x;
	float y;

	short Mx;
	short My;

	short xx;
	short yy;

	fRGB Color;
	unsigned long Ps;

	while(l)
	{
		l=(l>>1);
		MaxLevel++;
	}

	Data=(RGB **)Heap.Allocate(MaxLevel, sizeof(RGB *));
	L=(unsigned long *)Heap.Allocate(MaxLevel, sizeof(unsigned long));
	Lx=(float *)Heap.Allocate(MaxLevel, sizeof(float));
	Ly=(float *)Heap.Allocate(MaxLevel, sizeof(float));

	l=Max(Maxx, Maxy);
	for(i=MaxLevel;i>0;i--, l=(l>>1))
	{
		L[i-1]=l;
		Lx[i-1]=(float)l/(float)Maxx;
		Ly[i-1]=(float)l/(float)Maxy;
		
		dx=(float)Maxx/(float)l;
		dy=(float)Maxy/(float)l;

		Data[i-1]=(RGB *)Heap.Allocate(l*l, sizeof(RGB));

		x=0;
		for(Mx=0;Mx<l;Mx++, x+=dx)
		{
			y=0;
			for(My=0;My<l;My++, y+=dy)
			{
				if(Tiled)
				{
					Ps=0;
					Color=fRGB(0, 0, 0);

					if(!NonMean)
					{
						for(xx=x-dx;xx<x+dx;xx++)
							for(yy=y-dy;yy<y+dy;yy++)
							{
								Ps++;
								Color+=fRGB(_GetPixelTile((short)xx, (short)yy, Maxx, Maxy, ImData));
							}

						Color/=Ps;
					}
					else
						Color=fRGB(_GetPixelTile((short)x, (short)y, Maxx, Maxy, ImData));
				}
				else
				{
					Ps=0;
					Color=fRGB(0, 0, 0);

					if(!NonMean)
					{
						for(xx=x-dx;xx<x+dx;xx++)
							for(yy=y-dy;yy<y+dy;yy++)
							{
								Ps++;
								Color+=fRGB(_GetPixelCheck((short)xx, (short)yy, Maxx, Maxy, ImData));
							}

						Color/=Ps;
					}
					else
						Color=fRGB(_GetPixelCheck((short)x, (short)y, Maxx, Maxy, ImData));

				}

				Data[i-1][Mx+My*l]=Color.ToRGBCheck();
			}
		}
	}
}

void MIPMAP::Close()
{
	if(Data!=NULL)
	{
		for(unsigned short i=0;i<MaxLevel;i++)
			Heap.Free(Data[i]);

		Heap.Free(Data);
		Data=NULL;
	}

	Heap.Free(L);
	Heap.Free(Lx);
	Heap.Free(Ly);
	L=NULL;
	Lx=NULL;
	Ly=NULL;

	MaxLevel=0;
	Tiled=0;
}


// struct SURFACE

void SURFACE::Initialize(unsigned short Maxx, unsigned short Maxy, unsigned long (*tol)(const unsigned short, const unsigned short, const unsigned short))
{
	Close();

	if(tol==NULL)
		(*this).tol=_st_tol;
	else
		(*this).tol=tol;

	(*this).Maxx=Maxx;
	(*this).Maxy=Maxy;
	MaxT=Maxx*Maxy;

	CloseMipMap();
	CloseAlphaMap();

	Data=(RGB *)Heap.PAllocate(MaxT, sizeof(RGB));

	Log.Message("Created surface.");
}

void SURFACE::Initialize(SURFACE *Like)
{
	Initialize(Like->Maxx, Like->Maxy, Like->tol);
}

void SURFACE::Close()
{
	BOOL SendMsg=(Data!=NULL);

	Heap.Free(Data);
	Data=NULL;
	Maxx=Maxy=0;
	MaxT=0;
	CloseMipMap();
	CloseAlphaMap();

	if(SendMsg)
		Log.Message("Closed surface.");
}

void SURFACE::InitializeMipMap(BOOL Tiled, BOOL NonMean)
{
	CloseMipMap();

	MipMap=(MIPMAP *)Heap.Allocate(1, sizeof(MIPMAP));
	MipMap->Initialize(Tiled, NonMean, Maxx, Maxy, Data);

	Log.Message("Initialized MipMap.");
}

void SURFACE::CloseMipMap()
{
	if(MipMap!=NULL)
	{
		MipMap->Close();
		Heap.Free(MipMap);
		MipMap=NULL;

		Log.Message("Closed MipMap.");
	}
}

void SURFACE::InitializeAlphaMap(SURFACE *Map)
{
	CloseAlphaMap();

	AlphaMap=(uint8 *)Heap.PAllocate(MaxT, sizeof(uint8));

	if(Map!=NULL)
		for(unsigned long i=0;i<MaxT;i++)
			AlphaMap[i]=Map->Data[i].g;

	Log.Message("Initialized AlphaMap.");
}

void SURFACE::InitializeAlphaMap(char Filename[])
{
	CloseAlphaMap();

	AlphaMap=(uint8 *)Heap.PAllocate(MaxT, sizeof(uint8));

	SURFACE Map;
	Map.LoadFromBMP(Filename);

	for(unsigned long i=0;i<MaxT;i++)
		AlphaMap[i]=Map.Data[i].g;

	Log.Message("Initialized AlphaMap.");
}

void SURFACE::InitializeAlphaMap(uint8 Value)
{
	CloseAlphaMap();

	AlphaMap=(uint8 *)Heap.PAllocate(MaxT, sizeof(uint8));

	memset(AlphaMap, Value, MaxT);
}

void SURFACE::CloseAlphaMap()
{
	BOOL SendMsg=(AlphaMap!=NULL);

	Heap.Free(AlphaMap);
	AlphaMap=NULL;

	if(SendMsg)
		Log.Message("Closed AlphaMap.");
}

void SURFACE::InvertAlphaMap()
{
	for(unsigned long i=0;i<MaxT;i++)
		AlphaMap[i]=0xff-AlphaMap[i];
}

short SURFACE::ltoi(unsigned long l)
{
	return _ltoi(l, MipMap->MaxLevel);

	/* Old Code:
	short i=MipMap->MaxLevel;

	while(l)
	{
		l=l>>1;
		i--;
	}

	return i;
	*/
}

void SURFACE::DrawTo(unsigned short x, unsigned short y, SURFACE *Dest)
{
	unsigned short j;
	unsigned long screen_offset=Dest->tol(x, y, Dest->Maxx);
	unsigned long bitmap_offset=0;

	for(j=0;j<Maxy;j++)
	{
		memcpy(&Dest->Data[screen_offset], &Data[bitmap_offset], (Maxx<<1)+Maxx);

		bitmap_offset+=Maxx;
		screen_offset+=Dest->Maxx;
	}
}

void SURFACE::DrawATo(unsigned short x, unsigned short y, SURFACE *Dest)
{
	unsigned short i;
	unsigned short j;
	unsigned long screen_offset=Dest->tol(x, y, Dest->Maxx);
	unsigned long bitmap_offset=0;

	for(j=0;j<Maxy;j++)
	{
		for(i=0;i<Maxx;i++)
			Dest->PutPixel(screen_offset+i, GetAPixel(bitmap_offset+i));

		bitmap_offset+=Maxx;
		screen_offset+=Dest->Maxx;
	}
}

void SURFACE::LoadFromBMP(char Filename[]) // Only load from 24-bit (R8G8B8) bitmaps
{
	Close();

	FILE *File;
	long Index;
	unsigned long aux;

	// Open the file
	if((File=fopen(Filename, "rb"))==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	// Check to see if it's a valid bitmap file
	if(fgetc(File)!='B'||fgetc(File)!='M')
	{
		fclose(File);
		Log.Message("!FAIL! ", Filename, " is not a bitmap file.");
		exit(1);
	}

	// Read width, height and number of colors, skip the rest
	fskip(File, 16);
	
	fread(&aux, 4, 1, File);
	Maxx=(unsigned short)aux;
	
	fread(&aux, 4, 1, File);
	Maxy=(unsigned short)aux;
	
	fskip(File, 28);

	Initialize(Maxx, Maxy, NULL);

	// Read the bitmap
	for(Index=(Maxy-1)*Maxx;Index>=0;Index-=Maxx)
		fread(&Data[Index], 3, Maxx, File);

	fclose(File);

	Log.Message("Loaded bitmap file ", Filename);
}

void SURFACE::SaveToBMP(char Filename[])
{
	FILE *File=fopen(Filename, "wb");
	long Index;
	unsigned long aux;

	if(File==NULL)
	{
		Log.Message("!FAIL! Error opening output file ", Filename);
		exit(1);
	}

	uint8 Info[]=	{	0x42, 0x4d, 0x36, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00
					};

	fwrite(Info, 1, 18, File);

	aux=Maxx;
	fwrite(&aux, 4, 1, File);
	aux=Maxy;
	fwrite(&aux, 4, 1, File);

	fwrite(&Info[26], 1, 28, File);

	for(Index=(Maxy-1)*Maxx;Index>=0;Index-=Maxx)
		fwrite(&Data[Index], 3, Maxx, File);

	fclose(File);

	Log.Message("Saved bitmap file ", Filename);
}


} // namespace Graphics2D
} // namespace Graphics
