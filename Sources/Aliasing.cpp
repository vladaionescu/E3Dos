


#include "Aliasing.h"


namespace Graphics
{
namespace Graphics2D
{


// class ANTIALIASING

void ANTIALIASING::Initialize(SURFACE *Target)
{
	Close();

	(*this).Target=Target;
	
	Buffer=(RGB *)Heap.PAllocate(Target->MaxT, sizeof(RGB));

	Log.Message("Created FSAA object.");
}

void ANTIALIASING::Close()
{
	BOOL SendMsg=(Buffer!=NULL);

	Heap.Free(Buffer);
	Target=NULL;

	if(SendMsg)
		Log.Message("Closed FSAA object.");
}

void ANTIALIASING::Do()
{	
	unsigned long i, j;
	unsigned long aux;
	unsigned long aux2;
	uint8 r, g, b;
	RGB le, to, ri, bo, ac;

	// FIXME: Corners won't work
	/*
	// Top-Left Corner
	ri=Target->Data[1];
	bo=Target->Data[Target->Maxx];
	ac=Target->Data[0];
	r=(ri.r+bo.r+6*ac.r)>>3;
	g=(ri.g+bo.g+6*ac.g)>>3;
	b=(ri.b+bo.b+6*ac.b)>>3;
	Buffer[0]=RGB(r, g, b);
	*/

	// Top Margin
	aux=Target->Maxx-1;
	for(i=1;i<aux;i++)
	{
		le=Target->Data[i-1];
		ri=Target->Data[i+1];
		bo=Target->Data[i+Target->Maxx];
		ac=Target->Data[i];
		r=(le.r+ri.r+bo.r+5*ac.r)>>3;
		g=(le.g+ri.g+bo.g+5*ac.g)>>3;
		b=(le.b+ri.b+bo.b+5*ac.b)>>3;
		Buffer[i]=RGB(r, g, b);
	}

	/*
	// Top-Right Corner
	le=Target->Data[Target->Maxx-2];
	bo=Target->Data[2*Target->Maxx-1];
	ac=Target->Data[Target->Maxx-1];
	r=(le.r+bo.r+6*ac.r)>>3;
	g=(le.g+bo.g+6*ac.g)>>3;
	b=(le.b+bo.b+6*ac.b)>>3;
	Buffer[Target->Maxx-1]=RGB(r, g, b);
	*/

	// Left Margin
	aux=Target->Maxx*(Target->Maxy-1);
	for(i=Target->Maxx;i<aux;i+=Target->Maxx)
	{
		to=Target->Data[i-Target->Maxx];
		ri=Target->Data[i+1];
		bo=Target->Data[i+Target->Maxx];
		ac=Target->Data[i];
		r=(to.r+ri.r+bo.r+5*ac.r)>>3;
		g=(to.g+ri.g+bo.g+5*ac.g)>>3;
		b=(to.b+ri.b+bo.b+5*ac.b)>>3;
		Buffer[i]=RGB(r, g, b);
	}

	// Right Margin
	aux=Target->MaxT-2;
	for(i=Target->Maxx-1;i<aux;i+=Target->Maxx)
	{
		le=Target->Data[i-1];
		to=Target->Data[i-Target->Maxx];
		bo=Target->Data[i+Target->Maxx];
		ac=Target->Data[i];
		r=(le.r+to.r+bo.r+5*ac.r)>>3;
		g=(le.g+to.g+bo.g+5*ac.g)>>3;
		b=(le.b+to.b+bo.b+5*ac.b)>>3;
		Buffer[i]=RGB(r, g, b);
	}

	/*
	// Bottom-Left Corner
	to=Target->Data[Target->Maxx*(Target->Maxy-2)];
	ri=Target->Data[Target->Maxx*(Target->Maxy-1)+1];
	ac=Target->Data[Target->Maxx*(Target->Maxy-1)];
	r=(to.r+ri.r+6*ac.r)>>3;
	g=(to.g+ri.g+6*ac.g)>>3;
	b=(to.b+ri.b+6*ac.b)>>3;
	Buffer[Target->Maxx*(Target->Maxy-1)]=RGB(r, g, b);
	*/

	// Bottom Margin
	aux=Target->Maxx*Target->Maxy-1;
	for(i=Target->Maxx*(Target->Maxy-1)+1;i<aux;i++)
	{
		le=Target->Data[i-1];
		to=Target->Data[i-Target->Maxx];
		ri=Target->Data[i+1];
		ac=Target->Data[i];
		r=(le.r+to.r+ri.r+5*ac.r)>>3;
		g=(le.g+to.g+ri.g+5*ac.g)>>3;
		b=(le.b+to.b+ri.b+5*ac.b)>>3;
		Buffer[i]=RGB(r, g, b);
	}

	/*
	// Bottom-Right Corner
	le=Target->Data[Target->Maxx*Target->Maxy-2];
	to=Target->Data[Target->Maxx*(Target->Maxy-1)-1];
	ac=Target->Data[Target->Maxx*Target->Maxy-1];
	r=(le.r+to.r+6*ac.r)>>3;
	g=(le.g+to.g+6*ac.g)>>3;
	b=(le.b+to.b+6*ac.b)>>3;
	Buffer[Target->Maxx*Target->Maxy-1]=RGB(r, g, b);
	*/

	// The rest
	aux=Target->Maxx*(Target->Maxy-1)+1;
	for(i=Target->Maxx+1;i<aux;i+=Target->Maxx)
	{
		aux2=i+Target->Maxx-2;
		for(j=i;j<aux2;j++)
		{
			le=Target->Data[j-1];
			to=Target->Data[j-Target->Maxx];
			ri=Target->Data[j+1];
			bo=Target->Data[j+Target->Maxx];
			ac=Target->Data[j];
			r=(le.r+to.r+ri.r+bo.r+(ac.r<<2))>>3;
			g=(le.g+to.g+ri.g+bo.g+(ac.g<<2))>>3;
			b=(le.b+to.b+ri.b+bo.b+(ac.b<<2))>>3;
			Buffer[j]=RGB(r, g, b);
		}
	}

	// Swap AntiAliasing.Buffer with Target->Data
	RGB *auxp;
	auxp=Target->Data;
	Target->Data=Buffer;
	Buffer=auxp;
}


} // namespace Graphics2D
} // namespace Graphics
