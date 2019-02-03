


#ifndef _TEST_H_
#define _TEST_H_


namespace TestingArea
{


	unsigned long Getlong(float Value)
	{
		return *((unsigned long *)(&Value));
	}


}; // namespace TestingArea


#endif // #ifndef _TEST_H_
