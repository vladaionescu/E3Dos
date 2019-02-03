


#ifndef _ALIASING_H_
#define _ALIASING_H_


#include "Color.h"
#include "Surface.h"


namespace Graphics
{
namespace Graphics2D
{


class ANTIALIASING
{
private:

	RGB *Buffer;
	SURFACE *Target;


public:

	ANTIALIASING()
	{
		Buffer=NULL;
	}
	
	~ANTIALIASING()
	{
		Close();
	}
	
	void Initialize(SURFACE *Target);
	void Close();

	void Do();
};


} // namespace Graphics2D
} // namespace Graphics


#endif // #ifndef _ALIASING_H_
