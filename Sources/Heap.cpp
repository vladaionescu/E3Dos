


#include "Heap.h"


namespace Memory
{


// class HEAP

void * HEAP::Allocate(unsigned long NBlocks, unsigned long SizeBlock)
{
	void *Block=calloc(NBlocks, SizeBlock);

	if(Block==NULL)
	{
		Log.Message("Memory allocation error.");
		exit(1);
	}

	return Block;
}
	
void * HEAP::PAllocate(unsigned long NBlocks, unsigned long SizeBlock)
{
	unsigned long Product=NBlocks*SizeBlock;
	void *Block=calloc(NBlocks, SizeBlock);
	unsigned int i=1;

	if(Product<=65536)
		while((unsigned long)Block&0x0000ffff+Product>65536)
		{
			if((i++)>NoPAllocateTries)
				return Block;
			Block=realloc(&Block, Product);
		}

	return Block;
}

void * HEAP::HPAllocate()
{
	void *Block=malloc(65536);
	unsigned int i=1;

	while((unsigned long)Block&0x0000ffff)
	{
		if((i++)>NoHPAllocateTries)
			return Block;
		Block=realloc(&Block, 65536);
	}

	return Block;
}

void * HEAP::DOSAllocate(unsigned long Size, unsigned short *Sel)
{
	void * Block;
	unsigned short seg;

	_dos_memalloc((unsigned short)(Size>>4)+1, &seg, Sel);
	Block=(void *)((unsigned long)seg<<4);
	
	if(Block==NULL)
	{
		Log.Message("Memory allocation error.");
		exit(1);
	}
	
	return Block;
}

void HEAP::Free(void *Block)
{
	if(Block!=NULL)
		free(Block);
}

void HEAP::FFree(void *Block)
{
	free(Block);
}

void HEAP::DOSFree(unsigned short Sel)
{
	_dos_memfree(Sel);
}

HEAP Heap;


} // namespace Memory
