


#include "2DGraph.h"


#ifdef PIXEL_COUNTER
unsigned long PixelCount;
#endif // #ifdef PIXEL_COUNTER


namespace Graphics
{
namespace Graphics2D
{


// struct FRONTBUFFER

void FRONTBUFFER::Initialize(unsigned short Maxx, unsigned short Maxy)
{
	(*this).Maxx=Maxx;
	(*this).Maxy=Maxy;

	unsigned long Prod=(unsigned long)Maxx*(unsigned long)Maxy*3;
	(*this).MaxGran=(unsigned short)(Prod>>16)+1;
	(*this).LastGranLen=Prod&0x0000ffff;

	if(!LastGranLen)
	{
		MaxGran--;
		LastGranLen=0x00010000;
	}
}


// class GRAPH

void GRAPH::Initialize(unsigned short Mode)
{
	Close();

	switch(Mode)
	{
	case(0x10f): // 320x200, 24-bit
		BackBuffer.Initialize(320, 200, &SURFACE::_tol0x10f);
		FrontBuffer.Initialize(320, 200);
		break;

#ifdef EXTEND_MODES
	case(0x0): // Custom mode
		BackBuffer.Initialize(1600, 1200, &SURFACE::_st_tol);
		break;

	case(0x112): // 640x480, 24-bit
		BackBuffer.Initialize(640, 480, &SURFACE::_tol0x112);
		FrontBuffer.Initialize(640, 480);
		break;

	case(0x115): // 800x600, 24-bit
		BackBuffer.Initialize(800, 600, &SURFACE::_tol0x115);
		FrontBuffer.Initialize(800, 600);
		break;

	case(0x118): // 1024x768, 24-bit
		BackBuffer.Initialize(1024, 768, &SURFACE::_tol0x118);
		FrontBuffer.Initialize(1024, 768);
		break;

	case(0x11a): // 1280x1024, 24-bit (Not Suported)
		BackBuffer.Initialize(1280, 1024, &SURFACE::_tol0x11a);
		FrontBuffer.Initialize(1280, 1024);
		break;

	case(0x11f): // 1600x1200, 24-bit (Not Suported)
		BackBuffer.Initialize(1600, 1200, &SURFACE::_tol0x11f);
		FrontBuffer.Initialize(1600, 1200);
		break;
#endif // #ifdef EXTEND_MODES

	default:
		Log.Message("Unsuported Graphics Mode.");
		exit(1);
		break;
	}

	BackBuffer.Clear(0x00);
	Target=&BackBuffer;

#ifdef EXTEND_MODES
	if(Mode)
#endif // #ifdef EXTEND_MODES
	{
		// Try 0x62 for GeForce cards
		SetGraphicsMode(Mode);
		Log.Message("Entered graphics mode.");
	}
}

void GRAPH::Close()
{
	if(BackBuffer.Data!=NULL)
	{
		BackBuffer.Close();

		Log.Message("Closed Graph.");
	}
}

void GRAPH::SetGraphicsMode(unsigned short Mode)
{
	if(_GR_SetGraphicsMode(Mode)!=0x4f)
	{
		Log.Message("Unable to set graphics mode.");
		exit(1);
	}

	(*this).Mode=Mode;

	Log.Message("Switched to graphics mode ", (long)Mode);
}

void GRAPH::WaitVerticalRetrace()
{
	while((inp(0x03da)&0x08));
	while(!(inp(0x03da)&0x08));
}

void GRAPH::CopyBackBuffer()
{
	unsigned long Addr=(unsigned long)BackBuffer.Data;
	uint8 i;

	FrontBuffer.MaxGran--;
	for(i=0;i<FrontBuffer.MaxGran;i++)
	{
		_GR_SelectGranule(i);
		memcpy(FrontBuffer.Data, (void *)Addr, 65536);
		Addr+=65536;
	}
	_GR_SelectGranule(FrontBuffer.MaxGran);
	FrontBuffer.MaxGran++;

	memcpy(FrontBuffer.Data, (void *)Addr, FrontBuffer.LastGranLen);
}

void GRAPH::ClearScreen(uint8 Byte)
{
	uint8 i;

	FrontBuffer.MaxGran--;
	for(i=0;i<FrontBuffer.MaxGran;i++)
	{
		_GR_SelectGranule(i);
		memset(FrontBuffer.Data, Byte, 65536);
	}
	_GR_SelectGranule(FrontBuffer.MaxGran);
	FrontBuffer.MaxGran++;

	memset(FrontBuffer.Data, Byte, FrontBuffer.LastGranLen);
}

GRAPH Graph;


// struct LINE

void LINE::Draw(RGB Color)
{
	short i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx=b.x-a.x;
	dy=b.y-a.y;
	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=Sgn(dx);
	sdy=Sgn(dy);
	x=dyabs>>1;
	y=dxabs>>1;
	px=a.x;
	py=a.y;

	Graph.Target->PutPixelCheck(px, py, Color);

	if (dxabs>=dyabs)
		for(i=0;i<dxabs;i++)
		{
			y+=dyabs;
			if (y>=dxabs)
			{
				y-=dxabs;
				py+=sdy;
			}
			px+=sdx;
			Graph.Target->PutPixelCheck(px, py, Color);
		}
	else
		for(i=0;i<dyabs;i++)
		{
			x+=dxabs;
			if (x>=dyabs)
			{
				x-=dyabs;
				px+=sdx;
			}
			py+=sdy;
			Graph.Target->PutPixelCheck(px, py, Color);
		}
}


} // namespace Graphics2D
} // namespace Graphics
