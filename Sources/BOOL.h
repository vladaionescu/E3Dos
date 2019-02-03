

#ifndef _BOOL_H_
#define _BOOL_H_


#define BOOL char


namespace Misc
{


/*
struct BOOL
{
private:
	char info;

public:
	BOOL()
	{
		// info=0;
	}

	BOOL(short a)
	{
		(*this)=a;
	}

	~BOOL()
	{
	}

	short operator ! ()
	{
		return !(*this).info;
	}

	short operator & (BOOL a)
	{
		return (*this).info&&a.info;
	}

	short operator | (BOOL a)
	{
		return (*this).info||a.info;
	}

	short operator ^ (BOOL a)
	{
		return ((*this).info^a.info)&0x01;
	}

	short operator &= (BOOL a)
	{
		return (*this)=(*this)&a;
	}

	short operator |= (BOOL a)
	{
		return (*this)=(*this)|a;
	}

	short operator ^= (BOOL a)
	{
		return (*this)=(*this)^a;
	}

	short operator && (BOOL a)
	{
		return (*this).info&&a.info;
	}

	short operator || (BOOL a)
	{
		return (*this).info||a.info;
	}

	operator short ()
	{
		return (short)(*this).info;
	}

	short operator = (short a)
	{
		if(a)
		{
			(*this).info=1;
			return 1;
		}

		(*this).info=0;
		return 0;
	}

	void Toggle()
	{
		(*this).info=!(*this).info;
	}
};
*/


} // namespace Misc


#endif // #ifndef _BOOL_H_
