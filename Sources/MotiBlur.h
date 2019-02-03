


#ifndef _MOTIBLUR_H_
#define _MOTIBLUR_H_


#include "General.h"
#include "Color.h"
#include "Surface.h"


namespace Graphics
{
namespace Graphics2D
{


class MOTIONBLUR
{
private:
	RGB **BufferChain;
	uint8 Buffers;
	uint8 ActBuffer;
	SURFACE *Target;


public:

	MOTIONBLUR()
	{
		BufferChain=NULL;
		Buffers=0;
		Target=NULL;
	}
	
	~MOTIONBLUR()
	{
		Close();
	}

	void Initialize(uint8 Buffers, SURFACE *Target);
	void Close();

	void Do();
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _MOTIBLUR_H_
