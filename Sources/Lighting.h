


#ifndef _LIGHTING_H_
#define _LIGHTING_H_


#include "2DGraph.h"


using namespace Graphics::Graphics2D;
using namespace Geometry;


namespace Graphics
{
namespace Graphics3D
{
namespace Lighting
{


struct LIGHT
{
	VECTOR3 Direction;
	fRGB Diffuse;
#ifdef SpecularLighting
	fRGB Specular;
#endif // #ifdef SpecularLighting

	LIGHT()
	{
	}

#ifdef SpecularLighting
	LIGHT(const VECTOR3 &Direction, const fRGB &Diffuse, const fRGB &Specular)
#else // #ifdef SpecularLighting
	LIGHT(const VECTOR3 &Direction, const fRGB &Diffuse)
#endif // #else // #ifdef SpecularLighting
	{
		(*this).Direction=Direction/Direction.abs();
		(*this).Diffuse=Diffuse;
#ifdef SpecularLighting
		(*this).Specular=Specular;
#endif // #ifdef SpecularLighting
	}

	~LIGHT()
	{
	}
};


class LIGHTMANAGER
{
public:

	uint8 NoLights;
	LIGHT *Lights;
	fRGB Ambient;

#ifdef SpecularLighting
	// Specular exponent
	float Ns;
#endif // #ifdef SpecularLighting

	LIGHTMANAGER()
	{
		Lights=NULL;
		NoLights=0;
#ifdef SpecularLighting
		Ns=5;
#endif // #ifdef SpecularLighting
	}

	~LIGHTMANAGER()
	{
		Close();
	}

	void Initialize(uint8 NoLights);
	void Close();

	fRGB GetColor(const fRGB &NaturalColor, const VECTOR3 &Normal)
	{
		return MixLightWithColor(GetLightEffect(Normal), NaturalColor);
	}

	RGB MixLightWithColor(const fRGB &LightEffectColor, const fRGB &NaturalColor)
	{
#ifdef LightHDR
		return _MixLightWithColorAdd(LightEffectColor, NaturalColor);
#else // #ifdef LightHDR
		return _MixLightWithColorMultiply(LightEffectColor, NaturalColor);
#endif // #else // #ifdef LightHDR
	}

	ARGB MixLightWithColor(const fRGB &LightEffectColor, const ARGB NaturalColor)
	{
		float Alpha=NaturalColor.a
#ifdef LightHDR
			+
#else // #ifdef LightHDR
			*
#endif // #else // #ifdef LightHDR
			(LightEffectColor.r+LightEffectColor.g+LightEffectColor.b)/3;

		if(Alpha<0)
			Alpha=0;
		else if(Alpha>=256)
			Alpha=255;

#ifdef LightHDR
		return ARGB(Alpha, _MixLightWithColorAdd(LightEffectColor, fRGB(NaturalColor.r, NaturalColor.g, NaturalColor.b)));
#else // #ifdef LightHDR
		return ARGB(Alpha, _MixLightWithColorMultiply(LightEffectColor, fRGB(NaturalColor.r, NaturalColor.g, NaturalColor.b)));
#endif // #else // #ifdef LightHDR
	}

	RGB _MixLightWithColorMultiply(const fRGB &LightEffectColor, const fRGB &NaturalColor)
	{
		return (LightEffectColor*NaturalColor).ToRGBCheck();
	}

	RGB _MixLightWithColorAdd(const fRGB &LightEffectColor, const fRGB &NaturalColor)
	{
		return (LightEffectColor+NaturalColor).ToRGBCheck();
	}

	fRGB GetLightEffect(const VECTOR3 &Normal);
};


} // namespace Lighting
} // namespace Graphics3D
} // namespace Graphics

#endif // #ifndef _LIGHTING_H_
