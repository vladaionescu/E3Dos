


#ifndef _2DGRAPH_H_
#define _2DGRAPH_H_


// Special switches

//#define EXTEND_MODES
#define PIXEL_COUNTER


#ifdef PIXEL_COUNTER
extern unsigned long PixelCount;
#endif // #ifdef PIXEL_COUNTER


#include "Color.h"
#include "General.h"
#include "Geometry.h"
#include "Surface.h"
#include "Aliasing.h"
#include "MotiBlur.h"


// My first assembly language code:
void _GR_SelectGranule(uint8 Granule);
#pragma aux  _GR_SelectGranule =	\
	"movzx	edx, cl"				\
	"xor	bx, bx"					\
	"mov	ax, 0x4f05"				\
	"int	0x10"					\
	"mov	ax, 0x4f05"				\
	"inc	bl"						\
	"int	0x10"					\
	parm [cl]						\
	modify [ax bx bl edx];
/* Old Code:
{
	_asm{
		Mov     EDX, Granule
		Xor     BX, BX
		Mov     AX, 0x4f05
		Int     0x10
		Mov     AX, 0x4f05
		Inc     BL
		Int     0x10
	};
}
*/

unsigned short _GR_SetGraphicsMode(unsigned short Mode);
#pragma aux _GR_SetGraphicsMode =	\
	"mov	ax, 0x4f02"				\
	"int	0x10"					\
	parm	[bx]					\
	value	[ax]					\
	modify	[ax bx];
/* Old Code:
{
	int result;

	_asm{
		mov   ax, 0x4f02
		mov   bx, word ptr Mode
		int   0x10
		mov   word ptr result, ax
		mov   word ptr result+2, 0
	};

	return (result==0x4F);
}
*/


namespace Graphics
{
namespace Graphics2D
{


struct FRONTBUFFER
{
	RGB *Data;

	unsigned short Maxx;
	unsigned short Maxy;

	uint8 MaxGran;
	unsigned int LastGranLen;

	FRONTBUFFER()
	{
		Data=(RGB *)0xa0000;
	}

	~FRONTBUFFER()
	{
	}

	void Initialize(unsigned short Maxx, unsigned short Maxy);
};


class GRAPH
{
public:

	FRONTBUFFER FrontBuffer;
	SURFACE BackBuffer;
	SURFACE *Target;

	unsigned short MaxGran;
	unsigned int LastGranLen;

	unsigned short Mode;


	GRAPH()
	{
	}
	
	~GRAPH()
	{
		Close();
		SetGraphicsMode(0x03);
	}

	void Initialize(unsigned short Mode);
	void Close();

	void SetGraphicsMode(unsigned short Mode);

	void WaitVerticalRetrace();

	void CopyBackBuffer();
	void ClearScreen(uint8 Byte=0x00);
};

extern GRAPH Graph;


struct PIXEL
{
	short x;
	short y;

	PIXEL()
	{
	}
	
	PIXEL(short x, short y)
	{
		(*this).x=x;
		(*this).y=y;
	}
	
	PIXEL(VECTOR2 v)
	{
		(*this).x=v.x;
		(*this).y=v.y;
	}
	
	~PIXEL()
	{
	}
	
	operator VECTOR2 ()
	{
		return VECTOR2(x, y);
	}
	
	void Draw(RGB Color)
	{
		Graph.Target->PutPixelCheck(x, y, Color);
	}
	
	RGB Get()
	{
		return Graph.Target->GetPixelCheck(x, y);
	}
};


struct LINE
{
	PIXEL a;
	PIXEL b;

	LINE()
	{
	}
	
	LINE(short x1, short y1, short x2, short y2)
	{
		a.x=x1;
		a.y=y1;
	
		b.x=x2;
		b.y=y2;
	}
	
	LINE(PIXEL a, PIXEL b)
	{
		(*this).a=a;
		(*this).b=b;
	}
	
	~LINE()
	{
	}

	void Draw(RGB Color);
};


} // namespace Graphics2D
} // namespace Graphics


#include "Font.h"
#include "PBar.h"
#include "Trans.h"


#endif // #ifndef _2DGRAPH_H_
