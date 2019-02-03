


#ifndef _STARS_H_
#define _STARS_H_


namespace StarGenerator
{


float _DistributionCurve(float x)
{
	x=pow(x, 50)+x-1;

	if(x>1)
		x=1;

	return x;
}

void Generate(unsigned long NoStars) // Worked well with 30000 Stars
{
	SURFACE Bitmap;
	ANTIALIASING AA;

	Bitmap.Initialize(1440, 720, NULL);

	short x, y;
	RGB c;

	VECTOR2 P;
	float alpha;
	float r;

	Bitmap.Clear(Black);

	for(unsigned long i=0;i<NoStars;i++)
	{
		x=(short)Random.Rulong(0, Bitmap.Maxx);
		y=(short)Random.Rulong(0, Bitmap.Maxy);
		c=(fRGB(8, 8, 4)*Random.RCurve(0, 1, &_DistributionCurve)+fRGB(Random.Rfloat(0, 1), Random.Rfloat(0, 1), Random.Rfloat(0, 0.5))/4.).ToRGBCheck();

		for(unsigned short j=0;j<50;j++)
		{
			alpha=Random.Rfloat(0, 2*PI);
			r=Random.Rfloat(0, 1);
			P=VECTOR2(r*cos(alpha), r*sin(alpha));
			Bitmap.PutPixelCheck(x+P.x, y+P.y, ARGB((1-r)*255., c));
		}
	}

	Bitmap.SaveToBMP("BITMAPS\\StarTest.bmp");
}


} // namespace StarGenerator

#endif // #ifndef _STARS_H_