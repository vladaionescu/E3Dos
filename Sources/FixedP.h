


#ifndef _FIXEDP_H_
#define _FIXEDP_H_


#include "BOOL.h"


using namespace Misc;


// FIXME: WHY !!!!!!!!!!!!!!!!!!!!!!!?!?!?!?!?!?!?!???????????????????????????????? isn't it WORKING!!!!!!!???????
long _FP16_16_CONSFROM_FLOAT(unsigned long ulVal);
#pragma aux _FP16_16_CONSFROM_FLOAT =	\
	"cmp	eax, 0"						\
	"jz		short is_zero"				\
	"jg		short n_pos"				\
	"mov	edx, eax"					\
	"and	edx, 0x007fffff"			\
	"or		edx, 0x00800000"			\
	"and	eax, 0x7f800000"			\
	"shr	eax, 23"					\
	"sub	ax, 134"					\
	"cmp	ax, 0"						\
	"jl		short n_neg_e_neg"			\
	"shl	edx, al"					\
	"neg	edx"						\
	"jmp	short end"					\
"n_neg_e_neg:"							\
	"neg	ax"							\
	"shr	edx, al"					\
	"neg	edx"						\
	"jmp	short end"					\
"n_pos:"								\
	"mov	eax, edx"					\
	"and	edx, 0x007fffff"			\
	"or		edx, 0x00800000"			\
	"and	eax, 0x7f800000"			\
	"shr	eax, 23"					\
	"sub	ax, 134"					\
	"cmp	ax, 0"						\
	"jl		short n_pos_e_neg"			\
	"shl	edx, al"					\
	"jmp	short end"					\
"n_pos_e_neg:"							\
	"neg	ax"							\
	"shr	edx, al"					\
	"jmp	short end"					\
"is_zero:"								\
	"xor	edx, edx"					\
"end:"									\
	value [edx]							\
	parm [eax]							\
	modify [eax edx];

long _FP16_16_COPY_SIGN(unsigned long ulVal, long Mantissa);
#pragma aux _FP16_16_COPY_SIGN =	\
	"shr	eax, 31"				\
	"cmp	al, 0"					\
	"jz		short end"				\
	"neg	ebx"					\
"end:"								\
	value [ebx]						\
	parm [eax] [ebx]				\
	modify [eax ebx];

unsigned long _FP16_16_TOFLOAT(unsigned long ulVal);
#pragma aux _FP16_16_TOFLOAT =	\
	"cmp	eax, 0"				\
	"jz		short end"			\
	"mov	edx, eax"			\
	"shr	edx, 23"			\
	"sub	dl, 16"				\
	"shl	edx, 23"			\
	"and	eax, 0x007fffff"	\
	"or		eax, edx"			\
"end:"							\
	parm [eax]					\
	value [eax]					\
	modify [eax edx];

long _FP16_16_PROD(long a, long b);
#pragma aux _FP16_16_PROD =		\
	"sar	eax, 8"				\
	"sar	ebx, 8"				\
	"imul	ebx"				\
	"and	edx, 0x80000000"	\
	"or		eax, edx"			\
	parm [eax] [ebx]			\
	value [eax]					\
	modify [eax ebx edx];

long _FP16_16_DIV(long a, long b);
#pragma aux _FP16_16_DIV =		\
	"cmp	eax, 0"				\
	"jge	short positive"		\
	"neg	eax"				\
	"mov	ecx, eax"			\
	"shr	ecx, 16"			\
	"movzx	edx, cx"			\
	"shl	eax, 16"			\
	"neg	edx"				\
	"not	eax"				\
	"inc	eax"				\
	"jo		short division"		\
	"dec	edx"				\
	"jmp	short division"		\
"positive:"						\
	"mov	ecx, eax"			\
	"shr	ecx, 16"			\
	"movzx	edx, cx"			\
	"shl	eax, 16"			\
"division:"						\
	"idiv	ebx"				\
	parm [eax] [ebx]			\
	value [eax]					\
	modify [eax edx ecx];


namespace FixedPoint
{


/*
Some theory about Floating Point Numbers (single):

s23e8:
----------------------------------------------
| Number=(-1)^Sgn*Mantissa*2^(Exponent-150); |
----------------------------------------------


Where

Sgn=((b&0x80000000)>>31);
Exponent=((b&0x7f800000)>>23);
Mantissa=(b&0x007fffff)|0x00800000;

b is the binary form (unsigned long) of the floating point number.



A good thing to remember:
This machine is (and most of others are) LITTLE ENDIAN.
*/

struct FLOAT
{
	union
	{
		float fVal;
		unsigned long ulVal;
	};

	FLOAT()
	{
	}

	FLOAT(const float fVal)
	{
		(*this).fVal=fVal;
	}

	FLOAT(const unsigned long ulVal)
	{
		(*this).ulVal=ulVal;
	}

	// For (float)0: Sign=0, Exponent=-150, Mantissa=0
	FLOAT(BOOL Sign, short Exponent, unsigned long Mantissa)
	{
		// 31 | 30 29 28 27 26 25 24 23 | 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
		//  - |  -  -  -  -  -  -  -  - |  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
		// Sgn|  E  x  p  o  n  e  n  t |  -  -  -  -  -  -  -  M  a  n  t  i  s  s  a  -  -  -  -  -  -  -  -

		// Mantissa bit no. 23 (0x00800000) MUST be 1 (at least theoretically)

		ulVal=(unsigned long)(Sign<<31)|((unsigned long)(Exponent+150)<<23)|(Mantissa&0x007fffff);
	}

	~FLOAT()
	{
	}

	BOOL Sign()
	{
		return (BOOL)((ulVal&0x80000000)>>31);
	}

	unsigned long Mantissa()
	{
		return ((ulVal&0x007fffff)|0x00800000);
	}

	short Exponent()
	{
		return (short)(((ulVal&0x7f800000)>>23)-150);
	}

	//FLOAT operator = (const float a)
	//{
	//	(*this).fVal=a;

	//	return (*this);
	//}

	operator float ()
	{
		return (*this).fVal;
	}
};


// !Warning! Unpredicted results may occur if overflowed
struct FP16_16
{
	long Mantissa;

	FP16_16()
	{
	}

	FP16_16(const float fVal)
	{
		// FIXME:
		//Mantissa=_FP16_16_CONSFROM_FLOAT(FLOAT(fVal).ulVal);

		register FLOAT f(fVal);

		if(!f.ulVal)
		{
			Mantissa=0;
			return;
		}

		short Exp=f.Exponent()+16;

		if(Exp<0)
			Mantissa=(f.Mantissa()>>(-Exp));
		else
			Mantissa=(f.Mantissa()<<Exp);

		Mantissa=_FP16_16_COPY_SIGN(f.ulVal, Mantissa);
	}

	~FP16_16()
	{
	}

	//FP16_16 operator = (const float a)
	//{
	//	return (*this)=FP16_16(a);
	//}

	operator float () const
	{
		return (float)FLOAT(_FP16_16_TOFLOAT(FLOAT((float)Mantissa).ulVal));
	}

	FP16_16 operator + () const
	{
		return (*this);
	}

	FP16_16 operator - () const
	{
		return FP16_16(-Mantissa);
	}

	FP16_16 operator + (const FP16_16 a) const
	{
		return FP16_16((*this).Mantissa+a.Mantissa);
	}

	FP16_16 operator - (const FP16_16 a) const
	{
		return FP16_16((*this).Mantissa-a.Mantissa);
	}

	FP16_16 operator * (const FP16_16 a) const
	{
		return FP16_16(_FP16_16_PROD((*this).Mantissa, a.Mantissa));
	}

	FP16_16 operator / (const FP16_16 a) const
	{
		return FP16_16(_FP16_16_DIV((*this).Mantissa, a.Mantissa));
	}

	FP16_16 operator += (const FP16_16 a)
	{
		return (*this)=(*this)+a;
	}

	FP16_16 operator -= (const FP16_16 a)
	{
		return (*this)=(*this)-a;
	}

	FP16_16 operator *= (const FP16_16 a)
	{
		return (*this)=(*this)*a;
	}

	FP16_16 operator /= (const FP16_16 a)
	{
		return (*this)=(*this)/a;
	}

	int operator == (const FP16_16 a) const
	{
		return (*this).Mantissa==a.Mantissa;
	}

	int operator != (const FP16_16 a) const
	{
		return (*this).Mantissa!=a.Mantissa;
	}

	int operator < (const FP16_16 a) const
	{
		return (*this).Mantissa<a.Mantissa;
	}

	int operator <= (const FP16_16 a) const
	{
		return (*this).Mantissa<=a.Mantissa;
	}

	int operator > (const FP16_16 a) const
	{
		return (*this).Mantissa>a.Mantissa;
	}

	int operator >= (const FP16_16 a) const
	{
		return (*this).Mantissa>=a.Mantissa;
	}

	FP16_16 operator << (int a) const
	{
		return FP16_16((*this).Mantissa<<a);
	}

	FP16_16 operator >> (int a) const
	{
		return FP16_16((*this).Mantissa>>a);
	}
};


} // namespace FixedPoint


#endif // #ifndef _FIXEDP_H_
