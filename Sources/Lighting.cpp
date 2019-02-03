


#include "Lighting.h"


namespace Graphics
{
namespace Graphics3D
{
namespace Lighting
{

// class LIGHTMANAGER

void LIGHTMANAGER::Initialize(uint8 NoLights)
{
	Close();

	(*this).NoLights=NoLights;

	Lights=(LIGHT *)Heap.Allocate(NoLights, sizeof(LIGHT));
	memset(Lights, 0, NoLights*sizeof(LIGHT));

	Ambient=fRGB(0.0f, 0.0f, 0.0f);

	Log.Message("Created Light Manager.");
}

void LIGHTMANAGER::Close()
{
	BOOL SendMsg=(Lights!=NULL);

	Heap.Free(Lights);
	Lights=NULL;
	NoLights=0;

	if(SendMsg)
		Log.Message("Closed Light Manager.");
}

fRGB LIGHTMANAGER::GetLightEffect(const VECTOR3 &Normal)
{
	fRGB Diffuse=fRGB(0.0f, 0.0f, 0.0f);

#ifdef SpecularLighting
	fRGB Specular=fRGB(0.0f, 0.0f, 0.0f);
	float Dot;
#endif // #ifdef SpecularLighting

	for(uint8 i=0;i<NoLights;i++)
	{
#ifdef SpecularLighting
		Dot=Fabs(Normal%Lights[i].Direction);
		Diffuse+=Lights[i].Diffuse*Dot;
		Specular+=Lights[i].Specular*Pow(Dot, Ns);
#else // #ifdef SpecularLighting
		Diffuse+=Lights[i].Diffuse*Fabs(Normal%Lights[i].Direction);
#endif // #else // #ifdef SpecularLighting
	}

#ifdef SpecularLighting
	return Ambient+Diffuse+Specular;
#else // #ifdef SpecularLighting
	return Ambient+Diffuse;
#endif // #else // #ifdef SpecularLighting
}


} // namespace Lighting
} // namespace Graphics3D
} // namespace Graphics
