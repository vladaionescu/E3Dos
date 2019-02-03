


#ifndef _SURFACE_H_
#define _SURFACE_H_


#include "General.h"
#include "Color.h"
#include "Geometry.h"


short _ltoi(unsigned long l, unsigned short MaxLevel);
#pragma aux _ltoi =		\
"start:"				\
	"cmp	ecx, 0"		\
	"jz		short end"	\
	"shr	ecx, 1"		\
	"dec	ax"			\
	"jmp	short start"\
"end:"					\
	parm [ecx] [ax]		\
	value [ax]			\
	modify [ax ecx];

void _PutPixel(unsigned long DataAddress, unsigned long Tol, unsigned long InfoAddress);
#pragma aux _PutPixel =				\
	"add	edi, eax"				\
	"shl	eax, 1"					\
	"add	edi, eax"				\
	"mov	ax, word ptr [esi]"		\
	"mov	word ptr [edi], ax"		\
	"mov	al, byte ptr [esi+2]"	\
	"mov	byte ptr [edi+2], al"	\
	parm [edi] [eax] [esi]			\
	modify [edi eax];


using namespace Geometry;


namespace Graphics
{
namespace Graphics2D
{


struct MIPMAP
{
	RGB **Data;
	unsigned short MaxLevel;
	unsigned long *L;
	float *Lx;
	float *Ly;
	BOOL Tiled;

	MIPMAP()
	{
		Data=NULL;
		MaxLevel=0;
		L=NULL;
		Lx=NULL;
		Ly=NULL;
		Tiled=0;
	}

	~MIPMAP()
	{
		Close();
	}

	void Initialize(BOOL Tiled, BOOL NonMean, unsigned short Maxx, unsigned short Maxy, RGB *ImData);
	void Close();

	RGB _GetPixelCheck(short x, short y, unsigned short Maxx, unsigned short Maxy, RGB *ImData)
	{
		if(x<0||x>=Maxx||y<0||y>=Maxy)
			return (RGB)0x000000;

		return ImData[x+y*Maxx];
	}

	RGB _GetPixelTile(short x, short y, unsigned short Maxx, unsigned short Maxy, RGB *ImData)
	{
		if(x>=0&&x<Maxx)
			if(y>=0&&y<Maxy)
				return ImData[x+y*Maxx];
			else
				y%=Maxy;
		else if(y>=0&&y<Maxy)
				x%=Maxx;
		else
		{
			x%=Maxx;
			y%=Maxy;
		}

		return ImData[x+y*Maxx];
	}
};


struct SURFACE
{
public:

	unsigned short Maxx;
	unsigned short Maxy;
	unsigned long MaxT;

	unsigned long (*tol)(const unsigned short x, const unsigned short y, const unsigned short Maxx);
	RGB *Data;

	MIPMAP *MipMap;
	uint8 *AlphaMap;

public:

	SURFACE()
	{
		Data=NULL;
		Maxx=Maxy=0;
		MaxT=0;

		MipMap=NULL;
		AlphaMap=NULL;
	}

	~SURFACE()
	{
		Close();
	}

	void Initialize(unsigned short Maxx, unsigned short Maxy, unsigned long (*tol)(const unsigned short, const unsigned short, const unsigned short));
	void Initialize(SURFACE *Like);
	void Close();

	void InitializeMipMap(BOOL Tiled=0, BOOL NonMean=0);
	void CloseMipMap();

	void InitializeAlphaMap(SURFACE *Map=NULL);
	void InitializeAlphaMap(char Filename[]);	
	void InitializeAlphaMap(uint8 Value);
	void CloseAlphaMap();
	void InvertAlphaMap();

	BOOL IsCreated()
	{
		return (Data!=NULL);
	}

	BOOL HasAlpha()
	{
		return (AlphaMap!=NULL);
	}

	BOOL IsOut(const short x, const short y)
	{
		return (BOOL)(x<0||x>=Maxx||y<0||y>=Maxy);
	}

	BOOL IsOut(const float x, const float y)
	{
		return (BOOL)(x<0||x>=Maxx||y<0||y>=Maxy);
	}

	BOOL IsOut(const short x1, const short y1, const short x2, const short y2)
	{
		return (BOOL)(x1<0||x2>=Maxx||y1<0||y2>=Maxy);
	}

	BOOL IsOut(const float x1, const float y1, const float x2, const float y2)
	{
		return (BOOL)(x1<0||x2>=Maxx||y1<0||y2>=Maxy);
	}

	void Tile(short &x, short &y)
	{
		if(x>=0&&x<Maxx)
			if(y>=0&&y<Maxy)
				return;
			else
				y%=Maxy;
		else if(y>=0&&y<Maxy)
				x%=Maxx;
		else
		{
			x%=Maxx;
			y%=Maxy;
		}
	}

	void Tile(float &x, float &y)
	{
		if(x>=0&&x<Maxx)
			if(y>=0&&y<Maxy)
				return;
			else
			{
                y/=Maxy;
				y-=floor(y);
				y*=Maxy;				
			}
		else if(y>=0&&y<Maxy)
		{
			x/=Maxx;
			x-=floor(x);
			x*=Maxx;
		}
		else
		{
			x/=Maxx;
			x-=floor(x);
			x*=Maxx;

			y/=Maxy;
			y-=floor(y);
			y*=Maxy;
		}
	}

	RGB GetPixel(const short x, const short y)
	{
		return GetPixel(tol(x, y, Maxx));
	}

	ARGB GetAPixel(const short x, const short y)
	{
		return GetAPixel(tol(x, y, Maxx));
	}

	RGB GetPixelCheck(const short x, const short y)
	{
		if(IsOut(x, y))
			return (RGB)0x000000;

		return GetPixel(tol(x, y, Maxx));
	}

	ARGB GetAPixelCheck(const short x, const short y)
	{
		if(IsOut(x, y))
			return (RGB)0x000000;

		return GetAPixel(tol(x, y, Maxx));
	}

	RGB GetPixelTile(short x, short y)
	{
		Tile(x, y);

		return GetPixel(tol(x, y, Maxx));
	}

	ARGB GetAPixelTile(short x, short y)
	{
		Tile(x, y);

		return GetAPixel(tol(x, y, Maxx));
	}

	RGB GetPixelCheckB(const VECTOR2 &Pos) // Bilinear filtering (and no mipmapping)
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);

		if(IsOut(ix, iy, ix+1, iy+1))
			return (RGB)0x000000;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		unsigned long Tol=tol(ix, iy, Maxx);

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(Tol)
#define c2 (fRGB)GetPixel(Tol+1)
#define c3 (fRGB)GetPixel(Tol+Maxx)
#define c4 (fRGB)GetPixel(Tol+Maxx+1)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixelTileB(const VECTOR2 &Pos) // Bilinear filtering (and no mipmapping)
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);
		short ixp1=ix+1;
		short iyp1=iy+1;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		Tile(ix, iy);
		Tile(ixp1, iyp1);

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(ix, iy)
#define c2 (fRGB)GetPixel(ixp1, iy)
#define c3 (fRGB)GetPixel(ix, iyp1)
#define c4 (fRGB)GetPixel(ixp1, iyp1)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixelCheck(const short x, const short y, const unsigned long l)
	{
		if(IsOut(x, y))
			return (RGB)0x000000;

		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

		return GetPixel(x, y, i);
	}

	RGB GetPixelTile(short x, short y, const unsigned long l)
	{
		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

		Tile(x, y);

		return GetPixel(x, y, i);
	}

	RGB GetPixelCheckB(const VECTOR2 &Pos, const unsigned long l) // Bilinear filtering (with mipmapping)
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);

		if(IsOut(ix, iy, ix+1, iy+1))
			return (RGB)0x000000;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(ix, iy, i)
#define c2 (fRGB)GetPixel(ix+1, iy, i)
#define c3 (fRGB)GetPixel(ix, iy+1, i)
#define c4 (fRGB)GetPixel(ix+1, iy+1, i)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixelTileB(const VECTOR2 &Pos, const unsigned long l) // Bilinear filtering (with mipmapping)
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);
		short ixp1=ix+1;
		short iyp1=iy+1;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

		Tile(ix, iy);
		Tile(ixp1, iyp1);

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(ix, iy, i)
#define c2 (fRGB)GetPixel(ixp1, iy, i)
#define c3 (fRGB)GetPixel(ix, iyp1, i)
#define c4 (fRGB)GetPixel(ixp1, iyp1, i)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixelCheckT(const VECTOR2 &Pos, const float l) // Trilinear Filtering
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);

		if(IsOut(ix, iy, ix+1, iy+1))
			return (RGB)0x000000;

		float i=ltoi(l);
		short ii=floor(i);

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;
		float fi=i-(float)ii;

#define w1 (1-fx)*(1-fy)*(1-fi)
#define w2 fx*(1-fy)*(1-fi)
#define w3 (1-fx)*fy*(1-fi)
#define w4 fx*fy*(1-fi)
#define w5 (1-fx)*(1-fy)*fi
#define w6 fx*(1-fy)*fi
#define w7 (1-fx)*fy*fi
#define w8 fx*fy*fi

#define c1 (fRGB)GetPixel(ix, iy, ii)
#define c2 (fRGB)GetPixel(ix+1, iy, ii)
#define c3 (fRGB)GetPixel(ix, iy+1, ii)
#define c4 (fRGB)GetPixel(ix+1, iy+1, ii)
#define c5 (fRGB)GetPixel(ix, iy, ii+1)
#define c6 (fRGB)GetPixel(ix+1, iy, ii+1)
#define c7 (fRGB)GetPixel(ix, iy+1, ii+1)
#define c8 (fRGB)GetPixel(ix+1, iy+1, ii+1)

		return (c1*w1+c2*w2+c3*w3+c4*w4+c5*w5+c6*w6+c7*w7+c8*w8).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4
#undef w5
#undef w6
#undef w7
#undef w8

#undef c1
#undef c2
#undef c3
#undef c4
#undef c5
#undef c6
#undef c7
#undef c8
	}

	RGB GetPixelTileT(const VECTOR2 &Pos, const float l) // Trilinear Filtering
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);
		short ixp1=ix+1;
		short iyp1=iy+1;

		float i=ltoi(l);
		short ii=floor(i);

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;
		float fi=i-(float)ii;

		Tile(ix, iy);
		Tile(ixp1, iyp1);

#define w1 (1-fx)*(1-fy)*(1-fi)
#define w2 fx*(1-fy)*(1-fi)
#define w3 (1-fx)*fy*(1-fi)
#define w4 fx*fy*(1-fi)
#define w5 (1-fx)*(1-fy)*fi
#define w6 fx*(1-fy)*fi
#define w7 (1-fx)*fy*fi
#define w8 fx*fy*fi

#define c1 (fRGB)GetPixel(ix, iy, ii)
#define c2 (fRGB)GetPixel(ixp1, iy, ii)
#define c3 (fRGB)GetPixel(ix, iyp1, ii)
#define c4 (fRGB)GetPixel(ixp1, iyp1, ii)
#define c5 (fRGB)GetPixel(ix, iy, ii+1)
#define c6 (fRGB)GetPixel(ixp1, iy, ii+1)
#define c7 (fRGB)GetPixel(ix, iyp1, ii+1)
#define c8 (fRGB)GetPixel(ixp1, iyp1, ii+1)

		return (c1*w1+c2*w2+c3*w3+c4*w4+c5*w5+c6*w6+c7*w7+c8*w8).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4
#undef w5
#undef w6
#undef w7
#undef w8

#undef c1
#undef c2
#undef c3
#undef c4
#undef c5
#undef c6
#undef c7
#undef c8
	}

	RGB GetPixelCheckO(const VECTOR2 &Pos, const unsigned long l) // Distance-Based Optimized for Good Quality-Speed Balance
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);

		if(IsOut(ix, iy, ix+1, iy+1))
			return (RGB)0x000000;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

		if(i!=MipMap->MaxLevel)
			return GetPixel(ix, iy, i);

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(ix, iy)
#define c2 (fRGB)GetPixel(ix+1, iy)
#define c3 (fRGB)GetPixel(ix, iy+1)
#define c4 (fRGB)GetPixel(ix+1, iy+1)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixelTileO(const VECTOR2 &Pos, const unsigned long l) // Distance-Based Optimized for Good Quality-Speed Balance
	{
		short ix=floor(Pos.x);
		short iy=floor(Pos.y);
		short ixp1=ix+1;
		short iyp1=iy+1;

		float fx=Pos.x-(float)ix;
		float fy=Pos.y-(float)iy;

		Tile(ix, iy);
		Tile(ixp1, iyp1);

		short i=ltoi(l);

		if(i<=0)
			return MipMap->Data[0][0];

		if(i!=MipMap->MaxLevel)
			return GetPixel(ix, iy, i);

#define w1 (1-fx)*(1-fy)
#define w2 fx*(1-fy)
#define w3 (1-fx)*fy
#define w4 fx*fy

#define c1 (fRGB)GetPixel(ix, iy)
#define c2 (fRGB)GetPixel(ixp1, iy)
#define c3 (fRGB)GetPixel(ix, iyp1)
#define c4 (fRGB)GetPixel(ixp1, iyp1)

		return (c1*w1+c2*w2+c3*w3+c4*w4).ToRGBCheck();

#undef w1
#undef w2
#undef w3
#undef w4

#undef c1
#undef c2
#undef c3
#undef c4
	}

	RGB GetPixel(const short x, const short y, const unsigned short MipMapLevel)
	{
		if(MipMapLevel>=MipMap->MaxLevel)
			return GetPixel(x, y);

		return MipMap->Data[MipMapLevel][(unsigned long)((float)x*MipMap->Lx[MipMapLevel])+MipMap->L[MipMapLevel]*(unsigned long)((float)y*MipMap->Ly[MipMapLevel])];
	}

	RGB GetPixel(const unsigned long Tol)
	{
		return Data[Tol];
	}

	ARGB GetAPixel(const unsigned long Tol)
	{
		return ARGB(AlphaMap[Tol], GetPixel(Tol));
	}

	void PutPixel(const short x, const short y, const RGB Color)
	{
		PutPixel(tol(x, y, Maxx), Color);
	}

	void PutPixelCheck(const short x, const short y, const RGB Color)
	{
		if(IsOut(x, y))
			return;

		PutPixel(tol(x, y, Maxx), Color);
	}

	void PutPixel(const short x, const short y, const ARGB Color)
	{
		unsigned long Tol=tol(x, y, Maxx);
		PutPixel(Tol, Color);
	}

	void PutPixelCheck(const short x, const short y, const ARGB Color)
	{
		if(IsOut(x, y))
			return;

		unsigned long Tol=tol(x, y, Maxx);
		PutPixel(Tol, Color);
	}

	void PutPixel(const unsigned long Tol, const RGB Color)
	{
		_PutPixel((unsigned long)Data, Tol, (unsigned long)&Color);

#ifdef PIXEL_COUNTER
		PixelCount++;
#endif // #ifdef PIXEL_COUNTER
	}

	void PutPixel(const unsigned long Tol, const ARGB Color)
	{
		_Mix((unsigned long)&Color, (unsigned long)&Data[Tol]);

#ifdef PIXEL_COUNTER
		PixelCount++;
#endif // #ifdef PIXEL_COUNTER
	}

	short ltoi(unsigned long l);

	float ltoi(const float l)
	{
		return MipMap->MaxLevel-1-Log2(l);
	}

	void Clear(const uint8 Byte=0x00)
	{
		memset(Data, Byte, (MaxT<<1)+MaxT);
	}

	void CopyTo(SURFACE *Dest) // Only call if you are sure that at least Dest->MaxT==(*this).MaxT if not Dest->Maxx==(*this).Maxx&&Dest->Maxy==(*this).Maxx
	{
		memcpy(Dest->Data, Data, (MaxT<<1)+MaxT);
	}

	void DrawTo(unsigned short x, unsigned short y, SURFACE *Dest); // This function doesn't check if the source surface fits in the destination surface
	void DrawATo(unsigned short x, unsigned short y, SURFACE *Dest); // This function doesn't check if the source surface fits in the destination surface

	void LoadFromBMP(char Filename[]);
	void SaveToBMP(char Filename[]);

	static unsigned long _tol0x10f(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<8)+(y<<6));
	}

#ifdef EXTEND_MODES
	static unsigned long _tol0x112(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<9)+(y<<7));
	}

	static unsigned long _tol0x115(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<9)+(y<<8)+(y<<5));
	}

	static unsigned long _tol0x118(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<10));
	}

	static unsigned long _tol0x11a(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<11)+(y<<9));
	}

	static unsigned long _tol0x11f(const unsigned short x, const unsigned short y, const unsigned short)
	{
		return (x+(y<<11)+(y<<10)+(y<<7));
	}
#endif // #ifdef EXTEND_MODES

	static unsigned long _st_tol(const unsigned short x, const unsigned short y, const unsigned short Maxx)
	{
		return x+y*Maxx;
	}
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _SURFACE_H_
