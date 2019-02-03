


#include "Font.h"


namespace Graphics
{
namespace Graphics2D
{


// class FONT

void FONT::Initialize(char Filename[], unsigned short dx, unsigned short dy, char Letters[], char AlphaFile[])
{
	Close();

	(*this).dx=dx;
	(*this).dy=dy;

	FontTable.LoadFromBMP(Filename);

	if(AlphaFile!=NULL)
		FontTable.InitializeAlphaMap(AlphaFile);

	unsigned short x=0;
	unsigned short y=0;

	Characters=(CHARACTER *)Heap.PAllocate(256, sizeof(CHARACTER));
	memset(Characters, 0, sizeof(CHARACTER)<<8);

	for(unsigned short i=0;Letters[i];i++)
		if(Letters[i]=='\n')
		{
			y+=dy;
			x=0;
		}
		else
		{
			Characters[Letters[i]]=CHARACTER(x, y);
			x+=dx;
		}


	if(AlphaFile!=NULL)
		Log.Message("Loaded font file ", Filename, " with alpha channel ", AlphaFile);
	else
		Log.Message("Loaded font file ", Filename);
}

void FONT::Initialize(RGB Color, unsigned short dx, unsigned short dy, char Letters[], char AlphaFile[])
{
	Close();

	(*this).Color=Color;

	(*this).dx=dx;
	(*this).dy=dy;

	SURFACE AlphaMap;
	AlphaMap.LoadFromBMP(AlphaFile);
	FontTable.MaxT=AlphaMap.MaxT;
	FontTable.Maxx=AlphaMap.Maxx;
	FontTable.Maxy=AlphaMap.Maxy;
	FontTable.tol=AlphaMap.tol;
	FontTable.InitializeAlphaMap(&AlphaMap);
	AlphaMap.Close();

	unsigned short x=0;
	unsigned short y=0;

	Characters=(CHARACTER *)Heap.PAllocate(256, sizeof(CHARACTER));
	memset(Characters, 0, sizeof(CHARACTER)<<8);

	for(unsigned short i=0;Letters[i];i++)
		if(Letters[i]=='\n')
		{
			y+=dy;
			x=0;
		}
		else
		{
			Characters[Letters[i]]=CHARACTER(x, y);
			x+=dx;
		}

	Log.Message("Loaded font alpha channel ", AlphaFile);
}

void FONT::Close()
{
	BOOL SendMsg=(Characters!=NULL);

	Heap.Free(Characters);
	Characters=NULL;

	FontTable.Close();

	if(SendMsg)
		Log.Message("Closed Font.");
}

void FONT::Type(PIXEL Position, char Text[])
{
	if(FontTable.HasAlpha())
		_TypeA(Position, Text);
	else
		Type(Graph.Target, Position, Text);
}

void FONT::Type(SURFACE *Surface, PIXEL Position, char Text[])
{
	if(FontTable.HasAlpha())
	{
		_TypeA(Surface, Position, Text);
		return;
	}


	short Left=Position.x;

	for(unsigned short i=0;Text[i];i++)
		if(Text[i]=='\n')
		{
			Position.y+=dy;
			Position.x=Left;
		}
		else
		{
			if(Position.x>=0&&Position.y>=0&&Position.x+dx<Surface->Maxx&&Position.y+dy<Surface->Maxy)
			{
				unsigned short j;
				unsigned long screen_offset=Surface->tol(Position.x, Position.y, Surface->Maxx);
				unsigned long bitmap_offset=FontTable.tol(Characters[Text[i]].x, Characters[Text[i]].y, FontTable.Maxx);

				for(j=0;j<dy;j++)
				{
					memcpy(&Surface->Data[screen_offset], &FontTable.Data[bitmap_offset], (dx<<1)+dx);

					bitmap_offset+=FontTable.Maxx;
					screen_offset+=Surface->Maxx;
				}
			}

			Position.x+=dx;
		}
}

void FONT::_TypeA(PIXEL Position, char Text[])
{
	_TypeA(Graph.Target, Position, Text);
}

void FONT::_TypeA(SURFACE *Surface, PIXEL Position, char Text[])
{
	short Left=Position.x;
	unsigned short i, j, k;
	unsigned long screen_offset;
	unsigned long bitmap_offset;

	if(FontTable.IsCreated())
		for(i=0;Text[i];i++)
			if(Text[i]=='\n')
			{
				Position.y+=dy;
				Position.x=Left;
			}
			else
			{
				if(Position.x>=0&&Position.y>=0&&Position.x+dx<Surface->Maxx&&Position.y+dy<Surface->Maxy)
				{
					screen_offset=Surface->tol(Position.x, Position.y, Surface->Maxx);
					bitmap_offset=FontTable.tol(Characters[Text[i]].x, Characters[Text[i]].y, FontTable.Maxx);

					for(j=0;j<dy;j++)
					{
						for(k=0;k<dx;k++)
							Surface->PutPixel(screen_offset+k, FontTable.GetAPixel(bitmap_offset+k));

						bitmap_offset+=FontTable.Maxx;
						screen_offset+=Surface->Maxx;
					}
				}

				Position.x+=dx;
			}
	else
		for(i=0;Text[i];i++)
			if(Text[i]=='\n')
			{
				Position.y+=dy;
				Position.x=Left;
			}
			else
			{
				if(Position.x>=0&&Position.y>=0&&Position.x+dx<Surface->Maxx&&Position.y+dy<Surface->Maxy)
				{
					screen_offset=Surface->tol(Position.x, Position.y, Surface->Maxx);
					bitmap_offset=FontTable.tol(Characters[Text[i]].x, Characters[Text[i]].y, FontTable.Maxx);

					for(j=0;j<dy;j++)
					{
						for(k=0;k<dx;k++)
							Surface->PutPixel(screen_offset+k, ARGB(FontTable.AlphaMap[bitmap_offset+k], Color));

						bitmap_offset+=FontTable.Maxx;
						screen_offset+=Surface->Maxx;
					}
				}

				Position.x+=dx;
			}
}


} // namespace Graphics2D
} // namespace Graphics
