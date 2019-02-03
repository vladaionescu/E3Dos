


#include "Trans.h"


namespace Graphics
{
namespace Graphics2D
{
namespace TransitionEffects
{


// class FADE

void FADE::DrawFrame(float Step)
{
	unsigned long Tol;

	if(From!=NULL&&To!=NULL)
		for(Tol=0;Tol<Graph.Target->MaxT;Tol++)
			Graph.Target->PutPixel(Tol, (fRGB(From->Data[Tol])*(1.0f-Step)+fRGB(To->Data[Tol])*Step).ToRGBCheck());
	else if(From!=NULL)
		for(Tol=0;Tol<Graph.Target->MaxT;Tol++)
			Graph.Target->PutPixel(Tol, (fRGB(From->Data[Tol])*(1.0f-Step)+fRGB(ColorTo)*Step).ToRGBCheck());
	else if(To!=NULL)
		for(Tol=0;Tol<Graph.Target->MaxT;Tol++)
			Graph.Target->PutPixel(Tol, (fRGB(ColorFrom)*(1.0f-Step)+fRGB(To->Data[Tol])*Step).ToRGBCheck());
	else
		for(Tol=0;Tol<Graph.Target->MaxT;Tol++)
			Graph.Target->PutPixel(Tol, (fRGB(ColorFrom)*(1.0f-Step)+fRGB(ColorTo)*Step).ToRGBCheck());
}


// class T_EFFECT_1

void T_EFFECT_1::DrawFrame(float Step)
{
	short x=Step*(Graph.Target->Maxx+(RayDelta<<1))-RayDelta;
	short tx, ty;

	for(tx=0;tx<x-RayDelta;tx++)
		for(ty=0;ty<Graph.Target->Maxy;ty++)
			Graph.Target->PutPixelCheck(tx, ty, To->GetPixelCheck(tx, ty));

	for(tx=x-RayDelta;tx<x;tx++)
		for(ty=0;ty<Graph.Target->Maxy;ty++)
			Graph.Target->PutPixelCheck(tx, ty, (fRGB(To->GetPixelCheck(tx, ty))+RayColor*Pow((float)(tx-x+RayDelta)/(float)RayDelta, Ns)).ToRGBCheck());

	for(ty=0;ty<Graph.Target->Maxy;ty++)
		Graph.Target->PutPixelCheck(x, ty, RayColor.ToRGBCheck());

	for(tx=x+1;tx<x+RayDelta;tx++)
		for(ty=0;ty<Graph.Target->Maxy;ty++)
			Graph.Target->PutPixelCheck(tx, ty, (fRGB(From->GetPixelCheck(tx, ty))+RayColor*Pow((float)(x+RayDelta-tx)/(float)RayDelta, Ns)).ToRGBCheck());

	for(tx=x+RayDelta;tx<Graph.Target->Maxx;tx++)
		for(ty=0;ty<Graph.Target->Maxy;ty++)
			Graph.Target->PutPixelCheck(tx, ty, From->GetPixelCheck(tx, ty));
}


} // namespace TransitionEffects
} // namespace Graphics2D
} // namespace Graphics
