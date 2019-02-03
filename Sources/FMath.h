


#ifndef _FMATH_H_
#define _FMATH_H_


#define PI2 6.283185307179586476925286766558f		// 2*PI
#define PI 3.141592653589793238462643383279502884f	// PI
#define PI2s3 2.094395102393195492308428922186f		// 2*PI/3
#define PIs2 1.5707963267948966192313216916395f		// PI/2
#define PIs3 1.047197551196597746154214461093f		// PI/3
#define PIs4 0.78539816339744830961566084581975f	// PI/4
#define PIs6 0.5235987755982988730771072305465f		// PI/6


// Trigonometric functions
#define Sin(alpha) (float)sin(alpha)
#define Cos(alpha) (float)cos(alpha)
#define Tan(alpha) (float)tan(alpha)
#define Asin(lambda) (float)asin(lambda)
#define Acos(lambda) (float)acos(lambda)
#define Atan(lambda) (float)atan(lambda)
#define Atan2(y, x) (float)atan2(y, x)

// Other math functions
#define Pow(x, y) (float)pow(x, y)
#define Sqrt(x) (float)sqrt(x)
#define Log(x) (float)log(x)
#define Log10(x) (float)log10(x)
#define Log2(x) (float)log2(x)
#define Fabs(x) (float)fabs(x)


// Own macros
#define Min(a, b) ((a>b)?b:a)
#define Max(a, b) ((a<b)?b:a)

#define Sgn(x) ((x<0)?-1:((x>0)?1:0))


inline float Sqr(float x)
{
	return x*x;
}

inline float Cube(float x)
{
	return x*x*x;
}


#endif // #ifndef _FMATH_H_
