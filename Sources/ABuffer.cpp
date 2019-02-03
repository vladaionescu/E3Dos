


#include "ABuffer.h"


namespace Graphics
{
namespace Graphics3D
{
namespace AlphaBuffering
{


void AddNode(_A_NODE **List, const _A_INFO &Info)
{
#ifdef PIXEL_COUNTER
	PixelCount++;
#endif // #ifdef PIXEL_COUNTER

	if(*List==NULL)
	{
		*List=(_A_NODE *)Heap.Allocate(1, sizeof(_A_NODE));
		(*List)->Info=Info;
		(*List)->Next=NULL;
		return;
	}

	_A_NODE *Aux, *Next;

	if((*List)->Info.z>Info.z)
	{
		Aux=(_A_NODE *)Heap.Allocate(1, sizeof(_A_NODE));
		Aux->Info=Info;
		Aux->Next=*List;
		*List=Aux;
		return;
	}

	Aux=*List;

	while(1)
	{
		if(Aux->Next==NULL||Aux->Next->Info.z>Info.z)
		{
			Next=Aux->Next;
			Aux->Next=(_A_NODE *)Heap.Allocate(1, sizeof(_A_NODE));
			Aux->Next->Info=Info;
			Aux->Next->Next=Next;
			return;
		}

		Aux=Aux->Next;
	}
}

void DestructList(_A_NODE **List)
{
	if(*List==NULL)
		return;

	DestructList(&(*List)->Next);
	Heap.FFree(*List);
}


} // namespace AlphaBuffering
} // namespace Graphics3D
} // namespace Graphics
