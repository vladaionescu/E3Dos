


#ifndef _HEAP_H_
#define _HEAP_H_


#include <malloc.h>
#include <mem.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include <i86.h>

#include "Logging.h"


void _dos_memalloc(unsigned short para, unsigned short *seg, unsigned short *sel);
#pragma aux _dos_memalloc =			\
	"push  ecx"						\
	"push  edx"						\
	"mov   ax, 0100h"				\
	"int   31h"						\
	"pop   ebx"						\
	"mov   [ebx], dx"				\
	"pop   ebx"						\
	"mov   [ebx], ax"				\
	parm   [bx] [ecx] [edx]			\
	modify [ax ebx ecx edx];

void _dos_memfree(unsigned short sel);
#pragma aux _dos_memfree =		\
	"mov ax, 0101h"				\
	"int 31h"					\
	parm [dx]					\
	modify [ax dx];


namespace Memory
{


#define NoPAllocateTries 4096
#define NoHPAllocateTries 1024


class HEAP
{
public:

	void * Allocate(unsigned long NBlocks, unsigned long SizeBlock); // Extended (Protected-mode, Upper) memory allocation
	void * PAllocate(unsigned long NBlocks, unsigned long SizeBlock); // Tries to allocate memory without Page Boundary crossing
	void * HPAllocate(); // Tries to allocate an entire Memory Page. !Beta!
	void * DOSAllocate(unsigned long Size, unsigned short *Sel); // DOS (Real-mode, Lower) memory allocation. !Beta! Only use with sizes lower than 32768

	void Free(void *Block);
	void FFree(void *Block);
	void DOSFree(unsigned short Sel);
};

extern HEAP Heap;


} // namespace Memory


#endif // #ifndef _HEAP_H_
