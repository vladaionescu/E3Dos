


#ifndef _MOVIE_H_
#define _MOVIE_H_


#include "2DGraph.h"


namespace Graphics
{
namespace Graphics2D
{
namespace Movie
{


class MOVIE
{
public:
	unsigned long Frame;
	unsigned long NoFrames;
	BOOL Created; // Do not modify by hand!

private:
	char Filename[256];


public:

	MOVIE()
	{
		Created=0;
	}

	~MOVIE()
	{
		if(Created)
			StopRecording();
	}

	void StartRecording(char Filename[]);
	void RecordFrame(SURFACE *Screen);
	void StopRecording();
};


} // namespace Movie
} // namespace Graphics2D
} // namespace Graphics


#endif // ifndef _MOVIE_H_
