


#include "PBar.h"


namespace Graphics
{
namespace Graphics2D
{


// class PROGRESS_BAR

void PROGRESS_BAR::InitializeBack(char Filename[], PIXEL PositionB, char AlphaFile[])
{
	(*this).PositionB=PositionB;
	Back.LoadFromBMP(Filename);

	if(AlphaFile!=NULL)
		Back.InitializeAlphaMap(AlphaFile);
}

void PROGRESS_BAR::InitializeFront(char Filename[], PIXEL PositionF, char AlphaFile[])
{
	(*this).PositionF=PositionF;
	Front.LoadFromBMP(Filename);

	if(AlphaFile!=NULL)
		Front.InitializeAlphaMap(AlphaFile);
}

void PROGRESS_BAR::DrawBack()
{
	if(Back.HasAlpha())
		_DrawABack();
	else
		Back.DrawTo(PositionB.x, PositionB.y, Graph.Target);
}

void PROGRESS_BAR::DrawFrontHoriz(float ProgressStart, float ProgressEnd)
{
	if(Front.HasAlpha())
	{
		_DrawAFrontHoriz(ProgressStart, ProgressEnd);
		return;
	}

	unsigned short dx=(ProgressEnd-ProgressStart)*Front.Maxx;
	short Sx=ProgressStart*Front.Maxx;

	unsigned short j;
	unsigned long screen_offset=Graph.Target->tol(PositionF.x+Sx, PositionF.y, Graph.Target->Maxx);
	unsigned long bitmap_offset=Sx;

	for(j=0;j<Front.Maxy;j++)
	{
		memcpy(&Graph.Target->Data[screen_offset], &Front.Data[bitmap_offset], (dx<<1)+dx);	

		bitmap_offset+=Front.Maxx;
		screen_offset+=Graph.Target->Maxx;
	}
}

void PROGRESS_BAR::DrawFrontVert(float ProgressStart, float ProgressEnd)
{
	if(Front.HasAlpha())
	{
		_DrawAFrontVert(ProgressStart, ProgressEnd);
		return;
	}

	unsigned short j;
	unsigned long bitmap_offset=(unsigned short)(ProgressStart*(float)Front.Maxy);
	unsigned long screen_offset=Graph.Target->tol(PositionF.x, PositionF.y+(unsigned short)bitmap_offset, Graph.Target->Maxx);
	unsigned short Maxy=(unsigned short)(ProgressEnd*(float)Front.Maxy)-(unsigned short)bitmap_offset;
	bitmap_offset*=Front.Maxx;

	for(j=0;j<Maxy;j++)
	{
		memcpy(&Graph.Target->Data[screen_offset], &Front.Data[bitmap_offset], (Front.Maxx<<1)+Front.Maxx);

		bitmap_offset+=Front.Maxx;
		screen_offset+=Graph.Target->Maxx;
	}
}

void PROGRESS_BAR::_DrawABack()
{
	Back.DrawATo(PositionB.x, PositionB.y, Graph.Target);
}

void PROGRESS_BAR::_DrawAFrontHoriz(float ProgressStart, float ProgressEnd)
{
	unsigned short i;
	unsigned short j;
	unsigned long bitmap_offset=(unsigned short)(ProgressStart*(float)Front.Maxx);
	unsigned long screen_offset=Graph.Target->tol(PositionF.x+(unsigned short)bitmap_offset, PositionF.y, Graph.Target->Maxx);
	unsigned short Maxx=(unsigned short)(ProgressEnd*(float)Front.Maxx)-(unsigned short)bitmap_offset;

	for(j=0;j<Front.Maxy;j++)
	{
		for(i=0;i<Maxx;i++)
			Graph.Target->PutPixel(screen_offset+i, Front.GetAPixel(bitmap_offset+i));

		bitmap_offset+=Front.Maxx;
		screen_offset+=Graph.Target->Maxx;
	}
}

void PROGRESS_BAR::_DrawAFrontVert(float ProgressStart, float ProgressEnd)
{
	unsigned short i;
	unsigned short j;
	unsigned long bitmap_offset=(unsigned short)(ProgressStart*(float)Front.Maxy);
	unsigned long screen_offset=Graph.Target->tol(PositionF.x, PositionF.y+(unsigned short)bitmap_offset, Graph.Target->Maxx);
	unsigned short Maxy=(unsigned short)(ProgressEnd*(float)Front.Maxy)-(unsigned short)bitmap_offset;
	bitmap_offset*=Front.Maxx;

	for(j=0;j<Maxy;j++)
	{
		for(i=0;i<Front.Maxx;i++)
			Graph.Target->PutPixel(screen_offset+i, Front.GetAPixel(bitmap_offset+i));

		bitmap_offset+=Front.Maxx;
		screen_offset+=Graph.Target->Maxx;
	}
}


} // namespace Graphics2D
} // namespace Graphics
