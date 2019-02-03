


#ifndef _ABUFFER_H_
#define _ABUFFER_H_


#include "2DGraph.h"


using namespace Graphics::Graphics2D;


namespace Graphics
{
namespace Graphics3D
{
namespace AlphaBuffering
{


struct _A_INFO
{
	ARGB Color;
	float z;

	_A_INFO()
	{
	}

	_A_INFO(ARGB Color, float z)
	{
		(*this).Color=Color;
		(*this).z=z;
	}

	~_A_INFO()
	{
	}
};


struct _A_NODE
{
	_A_INFO Info;
	_A_NODE *Next;
};


inline RGB GetColor(_A_NODE *List)
{
	if(List==NULL)
		return (RGB)0x000000;

	return Mix(List->Info.Color, GetColor(List->Next));
}

void AddNode(_A_NODE **List, const _A_INFO &Info);
void DestructList(_A_NODE **List);


} // namespace AlphaBuffering
} // namespace Graphics3D
} // namespace Graphics


#endif // #ifndef _ABUFFER_H_
