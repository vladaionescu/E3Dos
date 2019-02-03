


#ifndef _PBAR_H_
#define _PBAR_H_


#include "2DGraph.h"


namespace Graphics
{
namespace Graphics2D
{


class PROGRESS_BAR
{
public:
	SURFACE Back;
	SURFACE Front;
	PIXEL PositionB;
	PIXEL PositionF;

	void InitializeBack(char Filename[], PIXEL PositionB, char AlphaFile[]=NULL);
	void InitializeFront(char Filename[], PIXEL PositionF, char AlphaFile[]=NULL);

	void Close()
	{
		Back.Close();
		Front.Close();
	}

	void DrawBack();
	void DrawFrontHoriz(float ProgressStart, float ProgressEnd);
	void DrawFrontVert(float ProgressStart, float ProgressEnd);

private:
	void _DrawABack();
	void _DrawAFrontHoriz(float ProgressStart, float ProgressEnd);
	void _DrawAFrontVert(float ProgressStart, float ProgressEnd);
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _PBAR_H_
