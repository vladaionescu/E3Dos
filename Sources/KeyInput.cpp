


#include "KeyInput.h"


namespace Input
{
namespace KInput
{


void interrupt _KI_IntHandler()
{
	char key, z;

	key=(char)inp(0x60);
	z=(char)inp(0x61);
	outp(0x61, z|0x80);
	outp(0x61, z);

	if(key>0x7f)
	{
		key&=0x7f;
		
		KeyInput._KeyMap[key]=0;
	}
	else
	{		
		KeyInput._KeyMap[key]=1;

		if(KeyInput._KeyCount<_KeyBufSize)
		{
			KeyInput._KeyBuf[KeyInput._BufTail++]=key;

			if(KeyInput._BufTail==_KeyBufSize)
				KeyInput._BufTail=0;

			KeyInput._KeyCount++;
		}
	}
	
	outp(0x20, 0x20);
}


// class KEYINPUT

void KEYINPUT::Initialize()
{
	Close();
	
	_KeyMap=(char *)Heap.PAllocate(256, sizeof(char));
	_SymMap=(char *)Heap.PAllocate(256, sizeof(char));
	_KeyBuf=(char *)Heap.PAllocate(_KeyBufSize, sizeof(char));

	char aux[256]={"\0\0331234567890-=\b\tQWERTYUIOP[]\r\0ASDFGHJKL;\'`\0\\ZXCVBNM,./\0*\0 "};
	memcpy(_SymMap, aux, 256);

	_InstallIntHandler();
	while(IsAKeyPressed());

	Log.Message("Created Key Input device.");
}

void KEYINPUT::Close()
{
	if(_SymMap!=NULL)
	{		
		_UninstallIntHandler();
		Heap.Free(_KeyMap);
		Heap.Free(_SymMap);
		Heap.Free(_KeyBuf);

		_KeyMap=NULL;
		_SymMap=NULL;
		_KeyBuf=NULL;

		Log.Message("Closed Key Input device.");
	}
}

void KEYINPUT::_InstallIntHandler()
{
	uint8 i;
	int8 *_keyflags=(int8  *)0x417;
	
	while(*_keyflags & 0x0F);
		
	memset(_KeyMap, 0, sizeof(_KeyMap));
	
	for(i=KCapsLock;i!=0;i++)
		_SymMap[i]=0;
	
	_SymMap[KSecMinus]='-';
	_SymMap[KSecPlus]='+';
	
	_KeyCount=0;
	_BufHead=0;
	_BufTail=0;

	_disable(); // asm cli
	_OldIntHandler=_dos_getvect(9);	
	_dos_setvect(9, _KI_IntHandler);
	_enable(); // asm sti

	Log.Message("Installed Key Input interrupt handler.");
}

void KEYINPUT::_UninstallIntHandler()
{
	_disable(); // asm cli
	_dos_setvect(9, _OldIntHandler);
	_enable(); // asm sti

	Log.Message("Uninstalled Key Input interrupt handler.");
}

uint8 KEYINPUT::_GetKeyScan()
{
	uint8 r;

	while(!_KeyCount);

	r=_KeyBuf[_BufHead++];
	if(_BufHead==_KeyBufSize)
		_BufHead=0;

	_KeyCount--;

	return r;
}

BOOL KEYINPUT::IsAKeyPressed()
{
	uint8 i;

	for(i=1;i<=0x58;i++)
		if(_KeyMap[i])
			return 1;

	return 0;
}

void KEYINPUT::WaitKey()
{
	while(!IsAKeyPressed());
	GetFirstKey();
}

KEYINPUT KeyInput;


} // namespace Input
} // namespace KInput
