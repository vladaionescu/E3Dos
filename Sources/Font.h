


#ifndef _FONT_H_
#define _FONT_H_


#include "2DGraph.h"


namespace Graphics
{
namespace Graphics2D
{


class FONT
{
public:

	struct CHARACTER
	{
		unsigned short x;
		unsigned short y;

		CHARACTER()
		{
		}

		CHARACTER(unsigned short x, unsigned short y)
		{
			(*this).x=x;
			(*this).y=y;
		}

		~CHARACTER()
		{
		}
	};

	unsigned short dx;
	unsigned short dy;

	SURFACE FontTable;
	RGB Color;

	CHARACTER *Characters;

	FONT()
	{
		Characters=NULL;
	}

	~FONT()
	{
		Close();
	}

	void Initialize(char Filename[], unsigned short dx, unsigned short dy, char Letters[], char AlphaFile[]=NULL);
	void Initialize(RGB Color, unsigned short dx, unsigned short dy, char Letters[], char AlphaFile[]);
	void Close();

	void Type(PIXEL Position, char Text[]);
	void Type(SURFACE *Surface, PIXEL Position, char Text[]);

private:
	void _TypeA(PIXEL Position, char Text[]);
	void _TypeA(SURFACE *Surface, PIXEL Position, char Text[]);
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _FONT_H_
