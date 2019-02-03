


#ifndef _COLOR_H_
#define _COLOR_H_


#include "General.h"


void _Mix(unsigned long SrcAddress, unsigned long DestAddress);
#pragma aux _Mix =					\
	"mov	bl, byte ptr [esi+3]"	\
	"cmp	bl, 0"					\
	"jz		short end"				\
	"mov	bh, bl"					\
	"neg	bh"						\
	"mov	al, bl"					\
	"mul	byte ptr [esi]"			\
	"mov	dx, ax"					\
	"mov	al, bh"					\
	"mul	byte ptr [edi]"			\
	"add	ax, dx"					\
	"mov	byte ptr [edi], ah"		\
	"inc	esi"					\
	"inc	edi"					\
	"mov	al, bl"					\
	"mul	byte ptr [esi]"			\
	"mov	dx, ax"					\
	"mov	al, bh"					\
	"mul	byte ptr [edi]"			\
	"add	ax, dx"					\
	"mov	byte ptr [edi], ah"		\
	"inc	esi"					\
	"inc	edi"					\
	"mov	al, bl"					\
	"mul	byte ptr [esi]"			\
	"mov	dx, ax"					\
	"mov	al, bh"					\
	"mul	byte ptr [edi]"			\
	"add	ax, dx"					\
	"mov	byte ptr [edi], ah"		\
"end:"								\
	parm [esi] [edi]				\
	modify [ax bx dx edi esi];

void _MBMix(unsigned long SrcAddress, unsigned long DestAddress);
#pragma aux _MBMix =				\
	"mov	bl, byte ptr [esi+3]"	\
	"cmp	bl, 0"					\
	"jz		short end"				\
	"mov	bh, bl"					\
	"neg	bh"						\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [esi]"		\
	"mul	bl"						\
	"mov	dx, ax"					\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [edi]"		\
	"mul	bh"						\
	"add	ax, dx"					\
	"mov	byte ptr [edi], 0xff"	\
	"sub	byte ptr [edi], ah"		\
	"inc	esi"					\
	"inc	edi"					\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [esi]"		\
	"mul	bl"						\
	"mov	dx, ax"					\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [edi]"		\
	"mul	bh"						\
	"add	ax, dx"					\
	"mov	byte ptr [edi], 0xff"	\
	"sub	byte ptr [edi], ah"		\
	"inc	esi"					\
	"inc	edi"					\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [esi]"		\
	"mul	bl"						\
	"mov	dx, ax"					\
	"mov	al, 0xff"				\
	"sub	al, byte ptr [edi]"		\
	"mul	bh"						\
	"add	ax, dx"					\
	"mov	byte ptr [edi], 0xff"	\
	"sub	byte ptr [edi], ah"		\
"end:"								\
	parm [esi] [edi]				\
	modify [ax bx dx esi edi];


namespace Graphics
{
namespace Graphics2D
{


struct RGB
{
	uint8 b;
	uint8 g;
	uint8 r;

	RGB()
	{
		// color=0x00;
	}

	RGB(const unsigned long a)
	{
		(*this).r=(uint8)(a&0xff);
		(*this).g=(uint8)((a>>8)&0xff);
		(*this).b=(uint8)((a>>16)&0xff);
	}

	RGB(const uint8 r, const uint8 g, const uint8 b)
	{
		(*this).r=r;
		(*this).g=g;
		(*this).b=b;
	}

	~RGB()
	{
	}
	
	operator unsigned long () const
	{
		return (unsigned long)(b<<16)|(unsigned long)(g<<8)|(unsigned long)r;
	}	

	RGB operator = (const unsigned long a)
	{
		(*this).r=(uint8)(a&0xff);
		(*this).g=(uint8)((a>>8)&0xff);
		(*this).b=(uint8)((a>>16)&0xff);

		return (*this);
	}

	RGB operator = (const RGB color)
	{
		memcpy(this, &color, 3);
		return (*this);
	}
};


struct ARGB
{	
	union
	{
		struct
		{
			uint8 b;
			uint8 g;
			uint8 r;
			uint8 a;
		};

		unsigned long ulVal;
	};

	ARGB()
	{
		// color=0x00ff;
	}
	
	ARGB(const RGB color)
	{
		(*this).a=0xff;
		(*this).r=color.r;
		(*this).g=color.g;
		(*this).b=color.b;
	}
	
	ARGB(const uint8 a, const RGB color)
	{
		(*this).a=a;
		(*this).r=color.r;
		(*this).g=color.g;
		(*this).b=color.b;
	}
	
	ARGB(const uint8 a, const uint8 r, const uint8 g, const uint8 b)
	{
		(*this).a=a;
		(*this).r=r;
		(*this).g=g;
		(*this).b=b;
	}
	
	~ARGB()
	{
	}
	
	operator RGB () const
	{
		return RGB(r, g, b);
	}
	
	ARGB operator = (const RGB color)
	{
		(*this).a=0xff;
		(*this).r=color.r;
		(*this).g=color.g;
		(*this).b=color.b;

		return (*this);
	}
	
	ARGB operator = (const ARGB color)
	{
		memcpy(this, &color, 4);
		return (*this);
	}
};


struct fRGB
{
	float b;
	float g;
	float r;

	fRGB()
	{
	}

	fRGB(const float r, const float g, const float b)
	{
		(*this).r=r;
		(*this).g=g;
		(*this).b=b;
	}

	fRGB(const RGB Color)
	{
		(*this).r=Color.r;
		(*this).g=Color.g;
		(*this).b=Color.b;
	}

	~fRGB()
	{
	}

	fRGB operator + () const
	{
		return (*this);
	}

	fRGB operator - () const
	{
		return fRGB(-r, -g, -b);
	}

	fRGB operator + (const fRGB &a) const
	{
		return fRGB(r+a.r, g+a.g, b+a.b);
	}

	fRGB &operator += (const fRGB &a)
	{
		return (*this)=(*this)+a;
	}

	fRGB operator - (const fRGB &a) const
	{
		return fRGB(r-a.r, g-a.g, b-a.b);
	}

	fRGB &operator -= (const fRGB &a)
	{
		return (*this)=(*this)-a;
	}

	fRGB operator * (const float a) const
	{
		return fRGB(r*a, g*a, b*a);
	}

	fRGB &operator *= (const float a)
	{
		return (*this)=(*this)*a;
	}

	fRGB operator * (const fRGB &a) const
	{
		return fRGB(r*a.r, g*a.g, b*a.b);
	}

	fRGB &operator *= (const fRGB &a)
	{
		return (*this)=(*this)*a;
	}

	fRGB operator / (const float a) const
	{
		return fRGB(r/a, g/a, b/a);
	}

	fRGB &operator /= (const float a)
	{
		return (*this)=(*this)/a;
	}

	operator RGB () const
	{
		return RGB((uint8)r, (uint8)g, (uint8)b);
	}

	void CheckR8G8B8()
	{
		if(r>=256)
			r=255;
		else if(r<0)
			r=0;

		if(g>=256)
			g=255;
		else if(g<0)
			g=0;

		if(b>=256)
			b=255;
		else if(b<0)
			b=0;
	}

	RGB ToRGBCheck() const
	{
		fRGB Checked;
		Checked=(*this);
		Checked.CheckR8G8B8();

		return (RGB)Checked;
	}
};

inline fRGB operator * (const float a, const fRGB &Color)
{
	return fRGB(Color.r*a, Color.g*a, Color.b*a);
}

inline RGB Mix(ARGB Src, RGB Dest)
{
	_Mix((unsigned long)&Src, (unsigned long)&Dest);

	return Dest;

	/* Old Code:
	unsigned short a=Src.a;

	return RGB((a*Src.r+Dest.r*(0x0100-a))>>8, (a*Src.g+Dest.g*(0x0100-a))>>8, (a*Src.b+Dest.b*(0x0100-a))>>8);
	*/
}

inline RGB MBMix(ARGB Src, RGB Dest)
{
	_MBMix((unsigned long)&Src, (unsigned long)&Dest);

	return Dest;

	/* Old Code:
	return RGB(255-((Src.a*(255-Src.r)+(255-Dest.r)*(0x0100-Src.a))>>8), 255-((Src.a*(255-Src.g)+(255-Dest.g)*(0x0100-Src.a))>>8), 255-((Src.a*(255-Src.b)+(255-Dest.b)*(0x0100-Src.a))>>8));
	*/
}


enum RGBColors
{
	LightRed=		0x0000ff,
	Red=			0x0000bf,
	DarkRed=		0x00007f,

	LightGreen=		0x00ff00,
	Green=			0x00bf00,
	DarkGreen=		0x007f00,

	LightBlue=		0xff0000,
	Blue=			0xbf0000,
	DarkBlue=		0x7f0000,

	Black=			0x000000,
	White=			0xffffff,

	LightGray=		0xbfbfbf,
	Gray=			0x7f7f7f,
	DarkGray=		0x404040,

	LightMagenta=	0xff00ff,
	Magenta=		0x7f007f,
	DarkMagenta=	0x400040,

	LightYellow=	0x00ffff,
	Yellow=			0x00bfbf,
	DarkYellow=		0x007f7f,

	LightCyan=		0xffff00,
	Cyan=			0xbfbf00,
	DarkCyan=		0x7f7f00,
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _COLOR_H_
