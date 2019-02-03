


#include "3DGraph.h"


namespace Graphics
{
namespace Graphics3D
{


// class GRAPH3D

// Declaration of static members
LIGHTMANAGER GRAPH3D::LightManager;
VIEWPORT GRAPH3D::ViewPort;
TRANSFORMATION GRAPH3D::Transformation;
float *GRAPH3D::ZBuffer;
AlphaBuffering::_A_NODE **GRAPH3D::ABuffer;
#ifdef VERTEX_COUNTER
unsigned long GRAPH3D::VertCount;
#endif // #ifdef VERTEX_COUNTER

void GRAPH3D::Initialize()
{
	Close();
		
	ZBuffer=(float *)Heap.Allocate(Graph.Target->MaxT, sizeof(float));

#ifdef ABuffering
	ABuffer=(AlphaBuffering::_A_NODE **)Heap.Allocate(Graph.Target->MaxT, sizeof(AlphaBuffering::_A_NODE *));
	SetABufferNULL();
#endif // #ifdef ABuffering

	Log.Message("Created 3D device.");
}

void GRAPH3D::Close()
{
	BOOL SendMsg=((ZBuffer!=NULL)||(ABuffer!=NULL));

	Heap.Free(ZBuffer);
	ZBuffer=NULL;
	Heap.Free(ABuffer);
	ABuffer=NULL;

	if(SendMsg)
		Log.Message("Closed 3D device.");
}

static void GRAPH3D::DoABuffer()
{
	for(unsigned long i=0;i<Graph.Target->MaxT;i++)
	{
		Graph.Target->PutPixel(i, AlphaBuffering::GetColor(ABuffer[i]));
		AlphaBuffering::DestructList(&ABuffer[i]);
		ABuffer[i]=NULL;
	}

#ifdef PIXEL_COUNTER
	PixelCount-=Graph.Target->MaxT;
#endif // #ifdef PIXEL_COUNTER
}

static void GRAPH3D::SetABufferNULL()
{
	for(unsigned long i=0;i<Graph.Target->MaxT;i++)
		ABuffer[i]=NULL;
}

GRAPH3D Graph3D;


} // namespace Graphics3D
} // namespace Graphics
