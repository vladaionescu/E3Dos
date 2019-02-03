


#include "Movie.h"


namespace Graphics
{
namespace Graphics2D
{
namespace Movie
{


void MOVIE::StartRecording(char Filename[])
{
	if(Created)
		StopRecording();

	Created=1;

	NoFrames=0;
	strcpy((*this).Filename, Filename);

	Log.Message("Started recording in files ", Filename);
}

void MOVIE::RecordFrame(SURFACE *Screen)
{
	char aux[256], num[256];

	strcpy(aux, Filename);
	strcat(aux, " (");
	itoa(NoFrames, num, 10);
	strcat(aux, num);
	strcat(aux, ").bmp");

	Screen->SaveToBMP(aux);

	NoFrames++;
}

void MOVIE::StopRecording()
{
	if(!Created)
		return;

	Created=0;

	Log.Message("Stopped recording movie files. Frames recorded: ", (long)NoFrames);
}


} // namespace Movie
} // namespace Graphics2D
} // namespace Graphics
