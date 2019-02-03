


#ifndef _PLASMA_H_
#define _PLASMA_H_


namespace Plasma
{


class ENTITY
{
public:

	VECTOR2 Acceleration;
	VECTOR2 Speed;
	VECTOR2 Position;

	VECTOR2 Center;
	VECTOR2 CenterDest;

	fRGB Color;

	ENTITY()
	{
		ChangeCenterDest();

		Center=VECTOR2(0.0f, 0.0f);
		Speed=VECTOR2(0.0f, 0.0f);
		Position=VECTOR2(0.5f, 0.5f);

		Color=fRGB(Random.Rfloat(0.3f, 1.0f), Random.Rfloat(0.3f, 1.0f), Random.Rfloat(0.3f, 1.0f));
		Color*=255.0f;
	}

	~ENTITY()
	{
	}

	void CalculateNewCoords()
	{
		if((CenterDest-Center).sqabs()<0.0001f)
			ChangeCenterDest();

		MoveCenter();
		CalculateAcceleration();

		Speed+=Acceleration*dTime.Variation;
		Position+=Speed*dTime.Variation;
	}

	void ChangeCenterDest()
	{
		CenterDest=Random.RVECTOR2(VECTOR2(2.0f, 2.0f))-VECTOR2(0.5f, 0.5f);
	}

	void MoveCenter()
	{
		Center+=(CenterDest-Center)*dTime.Variation*0.1f*((CenterDest-Center).sqabs()+1.0f);
	}

	void CalculateAcceleration()
	{
		float dx=Center.x-Position.x;
		float dy=Center.y-Position.y;

		if((Speed.x<0.0f)!=(dx<0.0f))
			Acceleration.x=dx*0.1f*Sqr(dx);
		else
			Acceleration.x=dx*0.01f*Sqr(dx);

		if((Speed.y<0.0f)!=(dy<0))
			Acceleration.y=dy*0.1f*Sqr(dy);
		else
			Acceleration.y=dy*0.01f*Sqr(dy);
	}
};

#define NEntities 4

ENTITY Entities[NEntities];

float dx;
float dy;

float k;

void Initialize()
{
	dx=1.0f/Graph.Target->Maxx;
	dy=1.0f/Graph.Target->Maxy;

	k=Random.Rfloat(0.0f, 20.0f);
}

void DoPlasma()
{
	int x;
	int y;

	float xf;
	float yf;

	unsigned long Tol;

	uint8 i;

	register fRGB Color;
	register float Coef;

	for(i=0;i<NEntities;i++)
		Entities[i].CalculateNewCoords();

	for(y=0, yf=0, Tol=0; y<Graph.Target->Maxy; y++, yf+=dy)
		for(x=0, xf=0; x<Graph.Target->Maxx; x++, xf+=dx, Tol++)
		{
			//Color=fRGB(512, 512, 512);
			Coef=k+dTime.Elapsed*0.001f;
			Color.r=Coef;
			Color.g=(Coef*=Coef);
			Color.b=Sqr(Coef);

			/*
			Coef=pow(xf*29., 0.9*Sin(k+dTime.Elapsed*0.002));
			Color.r+=Coef;
			Color.g+=(Coef*=Coef);
			Color.b+=Sqr(Coef);

			Coef=pow(yf*30., 0.8*Sin(k+dTime.Elapsed*0.003));
			Color.r+=Coef;
			Color.g+=(Coef*=Coef);
			Color.b+=Sqr(Coef);
			*/

			Color.r=(Sin(Color.r)+1.0f)*128.0f;
			Color.g=(Sin(Color.g)+1.0f)*128.0f;
			Color.b=(Sin(Color.b)+1.0f)*128.0f;

			Coef=0;

			for(i=0;i<NEntities;i++)
			{
				Coef+=(VECTOR2(xf, yf)-Entities[i].Position).sqabs();
				Color.r=(Sin(Color.r*0.005f+Coef*9.0f)+1.0f)*128.0f;
				Color.g=(Sin(Color.g*0.0009f+Coef*8.0f)+1.0f)*128.0f;
				Color.b=(Sin(Color.b*0.003f+Coef*7.0f)+1.0f)*128.0f;
				//Color+=Entities[i].Color*pow(50., -(VECTOR2(x/320., y/200.)-Entities[i].Position).sqabs());
				//Color+=-Entities[i].Color*pow((VECTOR2(xf, yf)-Entities[i].Position).sqabs(), 0.5);

				/*
				Color+=fRGB(LightBlue)*((int)(Entities[i].Position.x*320)==x&&(int)(Entities[i].Position.y*200)==y);
				Color+=fRGB(LightRed)*((int)(Entities[i].Center.x*320)==x&&(int)(Entities[i].Center.y*200)==y);
				Color+=fRGB(LightGreen)*((int)(Entities[i].CenterDest.x*320)==x&&(int)(Entities[i].CenterDest.y*200)==y);
				*/
			}

			Coef/=NEntities;

			Color*=(2.0f+Sin(Coef))*0.33f;

			Graph.Target->PutPixel(Tol, Color.ToRGBCheck());
		}

	//Graph.WaitVerticalRetrace();
	//Graph.CopyBackBuffer();

	//dTime.Next();
}


} // namespace Plasma


#endif // #ifndef _PLASMA_H_
