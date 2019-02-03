



#ifndef _GENERAL_H_
#define _GENERAL_H_



#include "BOOL.h"
#include "Logging.h"
#include "Heap.h"
#include "FixedP.h"
#include "FMath.h"

#include <conio.h>
#include <fstream.h>
#include <malloc.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <mem.h>
#include <time.h>


#define int8 signed char
#define uint8 unsigned char


using namespace Memory;


namespace MiscFile
{


inline void fskip(FILE *File, unsigned long NBytes=1) // Warning! This is only for reading; Writing is Beta!
{
	fseek(File, NBytes, SEEK_CUR);
}


} // namespace MiscFile

using namespace MiscFile;


namespace Misc
{


inline short shRound(float a)
{
	float fa=floor(a);

	if(a-fa<0.5)
		return (short)fa;
	else
		return (short)fa+1;
}

inline long lRound(float a)
{
	float fa=floor(a);

	if(a-fa<0.5)
		return (long)fa;
	else
		return (long)fa+1;
}


inline void EmptyFunc(float)
{
}

inline void EmptyFunc(unsigned long)
{
}


} // namespace Misc

using namespace Misc;


#endif // #ifndef _GENERAL_H_

