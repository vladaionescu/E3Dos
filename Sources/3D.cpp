



//#define DivStereo
//#define ConvStereo


#include "2DGraph.h"
#include "3DGraph.h"
#include "KeyInput.h"
#include "Mouse.h"
#include "TimeF.h"
#include "SB.h"
#include "Random.h"
#include "Movie.h"


using namespace Geometry;

using namespace Graphics::Graphics2D;
using namespace Graphics::Graphics2D::Movie;
using namespace Graphics::Graphics3D;
using namespace Graphics::Graphics3D::Triangle3D;
using namespace Graphics::Graphics3D::Line3D;
using namespace Graphics::Graphics3D::HField;
using namespace Graphics::Graphics3D::Lighting;

using namespace SB;

using namespace Input::KInput;
using namespace Input::MInput;

using namespace TimeFunctions;

using namespace RandomGenerator;


#define MoveM   7.0f
#define Move    1.0f
#define MoveRot 50.0f

#define ObjDist 50.0f

#define EyeDists2 0.5f


dTIME dTime;

LINE_MESH LMesh;
//TRIATEX_MESH TMesh;

HEIGHTFIELD Field;

HEIGHTFIELD EarthField;
SURFACE EarthTex;

SOUND Sound1;
SOUND Sound2[4];

SURFACE Texture1;

SURFACE SkyTex;
TRITEX_MESH SkyMesh;

TRITEX_MESH MMesh;
SURFACE MTex;
VECTOR3 MPos=VECTOR3(-0.5f, 0.75f, 0.0f).Setabs(4000);

TRILGTEX_MESH Mesh1;

//TRI3D a(VECTOR3(0, 0, 0), VECTOR3(0, -199, 0), VECTOR3(-319, -199, 0)); 
//TRI3D b(VECTOR3(0, 0, 0), VECTOR3(-319, 0, 0), VECTOR3(-319, -199, 0));

MOTIONBLUR MotionBlur;
ANTIALIASING AntiAliasing;

MOVIE Mov;

FONT Font;

PROGRESS_BAR PBar;


#if defined(DivStereo)|defined(ConvStereo)
SURFACE HalfScreen;
#endif // #if defined(DivStereo)|defined(ConvStereo)


//#include "Plasma.h"


void DoLMesh(LINE3D &Line)
{
	Line.a+=VECTOR3(0.0f, 0.0f, ObjDist);
	Line.b+=VECTOR3(0.0f, 0.0f, ObjDist);

	float Prod=dTime.Variation*PI/18.0f;

	Line.a.RotateX(Prod);
	Line.b.RotateX(Prod);

	Line.a-=VECTOR3(0.0f, 0.0f, ObjDist);
	Line.b-=VECTOR3(0.0f, 0.0f, ObjDist);

	Line.Project();
}

void DoField(TRILGTEX3D &Tri)
{
	/*
	VECTOR2 Proj;

	Tri.a.RotateX(-PIs2);
	Tri.b.RotateX(-PIs2);
	Tri.c.RotateX(-PIs2);

	Proj=VECTOR2(Tri.a.x, Tri.a.y);
	Tri.a.z=10*(sin(Proj.x*3*PI2/320.+dTime.TElapsed/10.)+sin(Proj.y*PI2/200+dTime.TElapsed/20.));

	Proj=VECTOR2(Tri.b.x, Tri.b.y);
	Tri.b.z=10*(sin(Proj.x*3*PI2/320.+dTime.TElapsed/10.)+sin(Proj.y*PI2/200+dTime.TElapsed/20.));

	Proj=VECTOR2(Tri.c.x, Tri.c.y);
	Tri.c.z=10*(sin(Proj.x*3*PI2/320.+dTime.TElapsed/10.)+sin(Proj.y*PI2/200+dTime.TElapsed/20.));

	Tri.a.RotateX(PI/2);
	Tri.b.RotateX(PI/2);
	Tri.c.RotateX(PI/2);
	*/

	/* // Normals
	LINE3D(Tri.a+Tri.aN*50, Tri.a, LightRed).Project();
	LINE3D(Tri.b+Tri.bN*50, Tri.b, LightRed).Project();
	LINE3D(Tri.c+Tri.cN*50, Tri.c, LightRed).Project();
	*/

	Tri.Project();
}

void DoMMesh(TRITEX3D &Tri)
{
	/*
	float Prod=dTime.Variation*MoveRot/10000;
	Tri.a.RotateY(Prod);
	Tri.b.RotateY(Prod);
	Tri.c.RotateY(Prod);
	*/

	VECTOR3 Sum=MPos+Graph3D.ViewPort.Position;

	Tri.a+=Sum;
	Tri.b+=Sum;
	Tri.c+=Sum;

	Tri.Project();

	Tri.a-=Sum;
	Tri.b-=Sum;
	Tri.c-=Sum;
}

void DoSkyMesh(TRITEX3D &Tri)
{
	Tri.a+=Graph3D.ViewPort.Position;
	Tri.b+=Graph3D.ViewPort.Position;
	Tri.c+=Graph3D.ViewPort.Position;

	float Prod=dTime.Variation*sClockTick*PI2/86400.0f;

	Tri.a.RotateY(Prod);
	Tri.b.RotateY(Prod);
	Tri.c.RotateY(Prod);

	Tri.a.RotateZ(Prod);
	Tri.b.RotateZ(Prod);
	Tri.c.RotateZ(Prod);

	/*
	LINE3D(Tri.a, Tri.b, LightBlue).Project();
	LINE3D(Tri.b, Tri.c, LightBlue).Project();
	LINE3D(Tri.c, Tri.a, LightBlue).Project();
	*/

	Tri.Project();

	Tri.a-=Graph3D.ViewPort.Position;
	Tri.b-=Graph3D.ViewPort.Position;
	Tri.c-=Graph3D.ViewPort.Position;
}

void Tr_ProjectWithNormals(TRILGTEX3D &Tri)
{
	LINE3D(Tri.a, Tri.a+Tri.aN*5.0f, LightRed).Project();
	LINE3D(Tri.b, Tri.b+Tri.bN*5.0f, LightRed).Project();
	LINE3D(Tri.c, Tri.c+Tri.cN*5.0f, LightRed).Project();

	Tri.Project();
}


void WriteFPS()
{
	char aux[100];
	char aux2[10];

	aux[0]=0;

	strcat(aux, "        FPS: ");
	strcat(aux, gcvt(dTime.FPS(), 4, aux2));
	strcat(aux, "\nAverage FPS: ");
	strcat(aux, gcvt(dTime.AvgFPS(), 4, aux2));
#ifdef VERTEX_COUNTER
	strcat(aux, "\nVertices: ");
	strcat(aux, ultoa(Graph3D.VertCount, aux2, 10));
#endif // #ifdef VERTEX_COUNTER
#ifdef PIXEL_COUNTER
	strcat(aux, "\nPixels: ");
	strcat(aux, ultoa(PixelCount, aux2, 10));
#endif // #define PIXEL_COUNTER

	Font.Type(&Graph.BackBuffer, PIXEL(5, (Graph.Target->Maxy-1)-5-4*10), aux);
}


void Render()
{
	Graph.Target->Clear(0x00);
	Graph3D.Next();

	//Mesh1.TriTransform(&Mesh1.Tr_Project);
	Field.TriTransform(&Field.Tr_ProjectMipMapped);
	//EarthField.TriTransform(&EarthField.Tr_Project);
	//LMesh.Project();
	//MMesh.TriTransform(&DoMMesh);
	//SkyMesh.TriTransform(&DoSkyMesh);

#ifdef ABuffering
	Graph3D.DoABuffer();
#endif // #ifdef ABuffering
}


void Cleanup()
{
	Graph.Target=&Graph.BackBuffer;
	Graph.Target->Clear();
	Font.Type(PIXEL(15, Graph.Target->Maxy-45), "Closing ...");
	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.05);
	Graph.CopyBackBuffer();

	SoundBlaster.Close();
	KeyInput.Close();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.3);
	Graph.CopyBackBuffer();

	Font.Close();

#if defined(DivStereo)|defined(ConvStereo)
	HalfScreen.Close();
#endif // #if defined(DivStereo)|defined(ConvStereo)

	Field.Delete();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.35);
	Graph.CopyBackBuffer();

	EarthTex.Close();
	EarthField.Close();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.45);
	Graph.CopyBackBuffer();

	Mesh1.Delete();

	Sound1.Delete();
	Sound2[0].Delete();
	Sound2[1].Delete();
	Sound2[2].Delete();
	Sound2[3].Delete();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.55);
	Graph.CopyBackBuffer();

	Field.Close();
	Texture1.Close();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.60);
	Graph.CopyBackBuffer();

	SkyTex.Close();
	SkyMesh.Delete();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.65);
	Graph.CopyBackBuffer();

	MTex.Close();
	MMesh.Delete();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.70);
	Graph.CopyBackBuffer();

	LMesh.Delete();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.75);
	Graph.CopyBackBuffer();

	Graph3D.LightManager.Close();
	Graph3D.Close();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.80);
	Graph.CopyBackBuffer();

	AntiAliasing.Close();
	MotionBlur.Close();

	MouseInput.Close();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.85);
	Graph.CopyBackBuffer();

	PBar.Close();

	Graph.Close();

	Log.Close();
}

void StopRendering()
{
	TransitionEffects::T_EFFECT_1 Fade;
	SURFACE From;
	SURFACE To;

	From.Initialize(&Graph.BackBuffer);
	Graph.BackBuffer.CopyTo(&From);

	To.Initialize(&Graph.BackBuffer);
	Graph.Target=&To;

	Graph.Target->Clear(0x00);
	Font.Type(PIXEL(15, Graph.Target->Maxy-45), "Closing ...");
	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.00);

	Fade.Start(&From, &To);

	float Start=dTime.Elapsed;
	float Now=dTime.Elapsed;

	Graph.Target=&Graph.BackBuffer;

	do
	{
		Fade.DrawFrame((Now-Start)/10.0f);
		Graph.CopyBackBuffer();
		Graph.WaitVerticalRetrace();

		Now+=ClockTick/30.0f;
	}
	while((Now-Start)/10.0f<=1.0f);
}


void OperateInput()
{
	VECTOR3 aux;

	// MouseInput
	{
		MouseInput.Next();

		Graph3D.ViewPort.Direction.Rotate(MoveM*dTime.Variation*MouseInput.dx/350.0f, Graph3D.ViewPort.Up);

		aux=-Graph3D.ViewPort.Up; // VECTOR3(0.0f, -1.0f, 0.0f);
		aux.Setabs(Tan(MoveM*dTime.Variation*MouseInput.dy/350.0f));
		Graph3D.ViewPort.Direction+=aux;
	}

	// MouseInput-like keys
	{
		if(Pressed(KLeft))
			Graph3D.ViewPort.Direction.RotateY(-MoveRot*dTime.Variation/350.0f);

		if(Pressed(KRight))
			Graph3D.ViewPort.Direction.RotateY(MoveRot*dTime.Variation/350.0f);

		if(Pressed(KUp))
		{
			aux=VECTOR3(0.0f, 1.0f, 0.0f);
			aux.Setabs(tan(MoveRot*dTime.Variation/350.0f));
			Graph3D.ViewPort.Direction+=aux;
		}

		if(Pressed(KDown))
		{
			aux=VECTOR3(0.0f, -1.0f, 0.0f);
			aux.Setabs(tan(MoveRot*dTime.Variation/350.0f));
			Graph3D.ViewPort.Direction+=aux;
		}
	}

	// Exit
		if(Pressed(KEsc))
		{
			Log.Message("-------------- Loop Ended --------------");
			Log.Message("Last    FPS: ", dTime.FPS());
			Log.Message("Average FPS: ", dTime.AvgFPS());
			Log.Message("----------------------------------------");

			StopRendering();
			Cleanup();

			exit(0);
		}


	// Standard movement keys
	{
		if(Pressed(KA))
		{
			aux=Graph3D.ViewPort.Direction*Graph3D.ViewPort.Up;//VECTOR3(0.0f, 1.0f, 0.0f);
			aux.Setabs(Move*dTime.Variation);
			Graph3D.ViewPort.Position+=aux;
		}

		if(Pressed(KD))
		{
			aux=Graph3D.ViewPort.Direction*Graph3D.ViewPort.Up;//VECTOR3(0.0f, 1.0f, 0.0f);
			aux.Setabs(Move*dTime.Variation);
			Graph3D.ViewPort.Position-=aux;
		}

		if(Pressed(KW))
		{
			aux=Graph3D.ViewPort.Direction;
			aux.Setabs(Move*dTime.Variation);
			Graph3D.ViewPort.Position+=aux;
		}

		if(Pressed(KS))
		{
			aux=Graph3D.ViewPort.Direction;
			aux.Setabs(Move*dTime.Variation);
			Graph3D.ViewPort.Position-=aux;
		}
	}

	// Beta keys
	{
		if(Pressed(KQ))
			Graph3D.ViewPort.Up.Rotate(-MoveRot*dTime.Variation/350.0f, Graph3D.ViewPort.Direction);
	
		if(Pressed(KE))
			Graph3D.ViewPort.Up.Rotate(MoveRot*dTime.Variation/350.0f, Graph3D.ViewPort.Direction);
	}

	// Elevation keys
	{
		if(Pressed(K2))
		{
			aux=Graph3D.ViewPort.Up;
			aux.Setabs(Move*dTime.Variation);
			Graph3D.ViewPort.Position+=aux;
		}

		if(Pressed(KX))
		{
			aux=Graph3D.ViewPort.Up;
			aux.Setabs(-Move*dTime.Variation);
			Graph3D.ViewPort.Position+=aux;
		}
	}

	// HightField Generator keys
	{
		if(Pressed(KB))
		{
			Field.RotateX(PIs2);
			Field.CreateHeightField(100, 5.0f, 100.0f);
			Field.RotateX(-PIs2);

			while(Pressed(KB));
		}
	}

	// Sound keys
	{
		if(Pressed(KP))
		{
			SoundBlaster.Play(&Sound1);
			while(Pressed(KP));
		}

		if(Pressed(KO))
		{
			SoundBlaster.Play(&Sound2[0]);
			while(Pressed(KO));
		}
	
		if(Pressed(KL))
		{
			SoundBlaster.Stop();
			while(Pressed(KL));
		}
	}

	// Misc keys
	{
		// Screen Shot
		if(Pressed(KF10))
		{
			Graph.BackBuffer.SaveToBMP("SHOTS\\Shot.bmp");
			while(Pressed(KF10));
		}

		// Super Screen Shot
		if(Pressed(KF11))
		{
			SURFACE Buffer;
			SURFACE *LastTarget;

			Buffer.Initialize(1600, 1200, &SURFACE::_st_tol);
			LastTarget=Graph.Target;
			Graph.Target=&Buffer;

			Graph3D.Initialize();
			Render();

			ANTIALIASING FSAA;
			FSAA.Initialize(Graph.Target);
			FSAA.Do();
			FSAA.Do();
			FSAA.Do();
			FSAA.Do();
			FSAA.Close();

			Buffer.SaveToBMP("SHOTS\\SShot.bmp");

			Graph.Target=LastTarget;
			Buffer.Close();
			Graph3D.Initialize();

			while(Pressed(KF11));
		}
	}

	// Font Keys
		if(Pressed(KF))
			Font.Color=Random.RRGB();

	// Recording keys
	{
		if(Pressed(KF4))
		{
			Mov.StartRecording("MOVIES\\FRAMES\\f");
			while(Pressed(KF4));
		}

		if(Pressed(KF5))
		{
			Mov.StopRecording();
			while(Pressed(KF5));
		}
	}
}


void Loop()
{
	VECTOR3 aux;

	Log.Message("------------- Loop Started -------------");

	while(1)
	{
		OperateInput();

		//Field.RotateX(-PIs2);
		//Graph3D.ViewPort.Position.y=10.+Field.GetHeight(VECTOR2(Graph3D.ViewPort.Position.x, -Graph3D.ViewPort.Position.z));
		//Field.RotateX(PIs2);

		//Graph.Target=&Texture1;
		//Plasma::DoPlasma();
		//Graph.Target=&Graph.BackBuffer;

#if defined(DivStereo)|defined(ConvStereo)
		//Graph.Target=&HalfScreen; // Use in plasma effect demo

		aux=Graph3D.ViewPort.Direction*Graph3D.ViewPort.Up;
		aux.Setabs(EyeDists2);
#ifdef DivStereo
		Graph3D.ViewPort.Position+=aux;
#else // #ifdef DivStereo
		Graph3D.ViewPort.Position-=aux;
#endif // #else // #ifdef DivStereo
		Render();
		HalfScreen.DrawTo(0, 0, &Graph.BackBuffer);
#ifdef DivStereo
		Graph3D.ViewPort.Position-=aux+aux;
#else // #ifdef DivStereo
		Graph3D.ViewPort.Position+=aux+aux;
#endif // #else // #ifdef DivStereo
		Render();
		HalfScreen.DrawTo(HalfScreen.Maxx, 0, &Graph.BackBuffer);
#ifdef DivStereo
		Graph3D.ViewPort.Position+=aux;
#else // #ifdef DivStereo
		Graph3D.ViewPort.Position-=aux;
#endif // #else // #ifdef DivStereo
#else // #if defined(DivStereo)|defined(ConvStereo)
		Render();
#endif // #else // #if defined(DivStereo)|defined(ConvStereo)


		if(Clicked(MLeft))
			MotionBlur.Do();

		WriteFPS();

		if(Clicked(MRight))
			AntiAliasing.Do();


		//Graph.WaitVerticalRetrace();
		Graph.CopyBackBuffer();


		dTime.Next();

		if(Mov.Created)
		{
			dTime.Variation=ClockTick/29.9697f;
			Mov.RecordFrame(&Graph.BackBuffer);
		}
	}
}



//#include "Test.h"

using namespace FixedPoint;

void FPTest()
{
	FP16_16 a, b;

	char aux[100];

	a=FP16_16(0.0f);
	b=FP16_16(0.002773f);

	cout<<itoa(a.Mantissa, aux, 16)<<endl;
	cout<<itoa(b.Mantissa, aux, 16)<<endl<<endl;

	cout<<(float)a<<endl;
	cout<<(float)b<<endl<<endl;

	cout<<(float)(a-b)<<endl;
	cout<<(float)(a+b)<<endl<<endl;

	cout<<(float)(a*b)<<endl<<endl;

	cout<<(float)(a/b)<<endl<<endl;

	cout<<(float)(-(a*(-b)))<<endl<<endl;

	//cout<<(float)(-((-a)/b))<<endl<<endl;

	getch();
	exit(0);
}

void StartRendering()
{
	TransitionEffects::FADE Fade;
	SURFACE From;
	SURFACE To;

	Graph.Target=&Graph.BackBuffer;

	From.Initialize(Graph.Target);
	Graph.Target->CopyTo(&From);

	To.Initialize(Graph.Target);

	Fade.Start(&From, &To);

	float Start=dTime.Elapsed;
	float Now=dTime.Elapsed;

	do
	{
		OperateInput();
		Graph.Target=&To;
		Render();

		Graph.Target=&Graph.BackBuffer;
		Fade.DrawFrame((Now-Start)/10.0f);

		Graph.CopyBackBuffer();

		dTime.Next();
		Now=dTime.Elapsed;
	}
	while((Now-Start)/10.0f<=1.0f);
}

void main()//int argc, char *argv[])
{
	//FPTest();

	Log.Initialize("Log.log");
	Graph.Initialize(0x10f);

	Font.Initialize(RGB(96, 128, 160), 5, 10,
		" !\"#$%&\'[]*+,-./\n0123456789:;<=>?\n@ABCDEFGHIJKLMNO\nPQRSTUVWXYZ[\\]^_\n`abcdefghijklmno\npqrstuvwxyz{|}~",
		"FONTS\\F4A3.bmp");

	PBar.InitializeBack("BITMAPS\\PBar01B.bmp", PIXEL(15, Graph.Target->Maxy-30));
	PBar.InitializeFront("BITMAPS\\PBar01F.bmp", PIXEL(17, Graph.Target->Maxy-28), "BITMAPS\\PBar01FA.bmp");

	Font.Type(PIXEL(15, Graph.Target->Maxy-45), "Loading, please wait ...");
	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.1);
	Graph.CopyBackBuffer();

	SoundBlaster.Initialize(44100);

	KeyInput.Initialize();

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.25);
	Graph.CopyBackBuffer();

	//Plasma::Initialize();

	//TMesh.New(2);
	//TMesh.Tris[0]=TRIATEX3D(VECTOR3(0, 0, 0), VECTOR3(0, -199, 0), VECTOR3(-319, -199, 0), PIXEL(0, 0), PIXEL(0, 199), PIXEL(319, 199), &Texture1, PIXEL(0, 0), PIXEL(0, 199), PIXEL(319, 199), &MTex);
	//TMesh.Tris[1]=TRIATEX3D(VECTOR3(0, 0, 0), VECTOR3(-319, 0, 0), VECTOR3(-319, -199, 0), PIXEL(0, 0), PIXEL(319, 0), PIXEL(319, 199), &Texture1, PIXEL(0, 0), PIXEL(319, 0), PIXEL(319, 199), &MTex);

	Field.NewField(10, 10, 10.0f, 10.0f, VECTOR2(0.0f, 0.0f), 512/10.0f, 512/10.0f, &Texture1);
	//Field.DisplaceMap("BITMAPS\\BumpMap1.bmp", 0.0f, 20.0f);
	Field.RotateX(-PIs2);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.30);
	Graph.CopyBackBuffer();

	//Field.RotateX(PIs2);
	//Field.CreateHeightField(100, 5.0f, 100.0f);
	//Field.RotateX(-PIs2);

	EarthTex.LoadFromBMP("BITMAPS\\Earth.bmp");
	//EarthTex.InitializeMipMap();
	EarthField.NewField(100, 100, 5.12f, 2.56f, VECTOR2(0.0f, 0.0f), 5.12f, 2.56f, &EarthTex);
	EarthField.DisplaceMap("BITMAPS\\EarthB.bmp", 0, 30);
	EarthField.RotateX(-PIs2);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.40);
	Graph.CopyBackBuffer();

	Mesh1.LoadMesh("MESH\\Ship.bx");
	Mesh1.ChangeTexture(&MTex);
	//Mesh1.Scale(10);

	Sound1.LoadWAV("SOUNDS\\d44100.wav");

	Sound2[0].LoadWAV("SOUNDS\\MI1f4410.wav");
	Sound2[1].LoadWAV("SOUNDS\\MI2f4410.wav");
	Sound2[2].LoadWAV("SOUNDS\\MI3f4410.wav");
	Sound2[3].LoadWAV("SOUNDS\\MI4f4410.wav");

	Sound2[0].NextChunk=&Sound2[1];
	Sound2[1].NextChunk=&Sound2[2];
	Sound2[2].NextChunk=&Sound2[3];

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.55);
	Graph.CopyBackBuffer();

	Texture1.LoadFromBMP("BITMAPS\\Plywood.bmp");
	//Texture1.InitializeAlphaMap(&Texture1);
	Texture1.InitializeMipMap(1);

	//Texture1.Initialize(Graph.BackBuffer.Maxx/4, Graph.BackBuffer.Maxy/4, &SURFACE::_st_tol);//, &SURFACE::_tol0x10f);

	SkyTex.LoadFromBMP("BITMAPS\\Stars.bmp");
	//SkyTex.InitializeMipMap();
	//SkyTex.InitializeAlphaMap(&SkyTex);
	SkyMesh.LoadMesh("MESH\\Sphere1.bx");
	SkyMesh.ChangeTexture(&SkyTex);
	SkyMesh.Scale(30000.0f);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.60);
	Graph.CopyBackBuffer();

	MTex.LoadFromBMP("BITMAPS\\Moon.bmp");
	//MTex.InitializeAlphaMap(&MTex);
	//MTex.InitializeMipMap();
	MMesh.LoadMesh("MESH\\Sphere2O.bx");
	MMesh.ChangeTexture(&MTex);
	MMesh.RotateY(PIs4);
	MMesh.RotateZ(-PIs3);
	MMesh.Scale(1000.0f);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.70);
	Graph.CopyBackBuffer();

	LMesh.LoadMesh("MESH\\DNA.x");
	LMesh.RotateZ(PIs2);
	LMesh.Translate(VECTOR3(0.0f, 0.0f, -ObjDist));
	LMesh.ChangeEntireColor(Magenta);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.75);
	Graph.CopyBackBuffer();

#if defined(DivStereo)|defined(ConvStereo)
	Graph3D.ViewPort=VIEWPORT(PIs4, 1.0f, 1000.0f, 2.0f/3.0f,
		VECTOR3(10, 10,  -10),
        VECTOR3(10, -0.2, -1),
		VECTOR3(0, 1,  0));
#else // #if defined(DivStereo)|defined(ConvStereo)
	Graph3D.ViewPort=VIEWPORT(PIs2, 1.0f, 1000.0f, 4.0f/3.0f,
		VECTOR3(10, 10,  -10),
		VECTOR3(1, -0.2, -1),
		VECTOR3(0, 1,  0));

	/*
	Graph3D.ViewPort=VIEWPORT(PIs2, 1.0f, 1000.0f, 4.0f/3.0f,
		VECTOR3(0, 0, 0),
		VECTOR3(0, 0, -1),
		VECTOR3(0, 1,  0));
	*/
#endif // #else // #if defined(DivStereo)|defined(ConvStereo)

	Graph3D.Initialize();
	Graph3D.LightManager.Initialize(1);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.80);
	Graph.CopyBackBuffer();

	AntiAliasing.Initialize(&Graph.BackBuffer);
	MotionBlur.Initialize(2, &Graph.BackBuffer);

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 0.85);
	Graph.CopyBackBuffer();

	Graph3D.LightManager.Lights[0]=LIGHT(-MPos, fRGB(256.0f, 256.0f, 256.0f));
	Graph3D.LightManager.Ambient=fRGB(-256.0f, -256.0f, -256.0f);

	//Graph3D.Cull=&Graph3D.St_NoCull;
	//Graph3D.Clip3V=&Graph3D.St_NoClip3V;
	//Graph3D.ClipFP=&Graph3D.St_NoClipFP;

#if defined(DivStereo)|defined(ConvStereo)
	HalfScreen.Initialize(Graph.BackBuffer.Maxx>>1, Graph.BackBuffer.Maxy, NULL);
	Graph.Target=&HalfScreen;
#endif // #if defined(DivStereo)|defined(ConvStereo)

	PBar.DrawBack();
	PBar.DrawFrontHoriz(0, 1.0);
	Graph.CopyBackBuffer();

	MouseInput.Initialize();
	dTime.Reset();

	StartRendering();
	Loop();
}
