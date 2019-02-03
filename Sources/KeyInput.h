



#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_


#include "General.h"
#include "Heap.h"


namespace Input
{
namespace KInput
{


enum KeyCodes
{
	KEsc=		0x01, // Escape
	K1=			0x02, // 1 !
	K2=			0x03, // 2 @
	K3=			0x04, // 3 #
	K4=			0x05, // 4 $
	K5=			0x06, // 5 %
	K6=			0x07, // 6 ^
	K7=			0x08, // 7 &
	K8=			0x09, // 8 *
	K9=			0x0A, // 9 (
	K0=			0x0B, // 0 )
	KMinus=		0x0C, // - _
	KEqual=		0x0D, // = +
	KBackSpace=	0x0E, // BackSpace
	KTab=		0x0F, // Tab
	KQ=			0x10, // q Q
	KW=			0x11, // w W
	KE=			0x12, // e E
	KR=			0x13, // r R
	KT=			0x14, // t T
	KY=			0x15, // y Y
	KU=			0x16, // u U
	KI=			0x17, // i I
	KO=			0x18, // o O
	KP=			0x19, // p P
	KLBraket=	0x1A, // [ {
	KRBraket=	0x1B, // ] }
	KEnter=		0x1C, // Enter
	KCtrl=		0x1D, // Ctrl
	KA=			0x1E, // A a
	KS=			0x1F, // K s
	KD=			0x20, // D d
	KF=			0x21, // F f
	KG=			0x22, // G g
	KH=			0x23, // H h
	KJ=			0x24, // J j
	KK=			0x25, // K k
	KL=			0x26, // L l
	KSemicolon=	0x27, // ; :
	KQuote=		0x28, // ' "
	KApostrophe=0x29, // ` ~
	KLShift=	0x2A, // Left Shift
	KBackSlash=	0x2B, // \ |
	KZ=			0x2C, // Z z
	KX=			0x2D, // X x
	KC=			0x2E, // C c
	KV=			0x2F, // V v
	KB=			0x30, // B b
	KN=			0x31, // N n
	KM=			0x32, // M m
	KComma=		0x33, // , <
	KPoint=		0x34, // . >
	KSlash=		0x35, // / ?
	KRShift=	0x36, // Right Shift
	KAsterix=	0x37, // * on the numpad
	KAlt=		0x38, // Alt
	KSpace=		0x39, // Space (Blank)
	KCapsLock=	0x3A, // Capitals Lock
	KF1=		0x3B, // F1
	KF2=		0x3C, // F2
	KF3=		0x3D, // F3
	KF4=		0x3E, // F4
	KF5=		0x3F, // F5
	KF6=		0x40, // F6
	KF7=		0x41, // F7
	KF8=		0x42, // F8
	KF9=		0x43, // F9
	KF10=		0x44, // F10
	KNumLock=	0x45, // Numbers Lock
	KScrollLock=0x46, // Scroll Lock
	KHome=		0x47, // Home
	KUp=		0x48, // Up
	KPageUp=	0x49, // PageUp
	KSecMinus=	0x4A, // - on the numpad
	KLeft=		0x4B, // Left
	KFive=		0x4C, // 5 on the numpad
	KRight=		0x4D, // Right
	KSecPlus=	0x4E, // + on the numpad
	KEnd=		0x4F, // End
	KDown=		0x50, // Down
	KPageDown=	0x51, // PageDown
	KInsert=	0x52, // Insert
	KDelete=	0x53, // Delete
	KF11=		0x57, // F11
	KF12=		0x58  // F12
};


#define _KeyBufSize 16


void interrupt _KI_IntHandler();


class KEYINPUT
{
private:

	char *_SymMap;
	void interrupt (*_OldIntHandler)();
	int _BufHead;


public:

	char *_KeyMap;
	uint8 _KeyCount;
	int _BufTail;
	char *_KeyBuf;
	

	KEYINPUT()
	{
		_KeyMap=NULL;
		_SymMap=NULL;
		_KeyBuf=NULL;
	}
	
	~KEYINPUT()
	{
		Close();
	}

	BOOL IsCreated()
	{
		if(_SymMap==NULL)
			return 0;

		return 1;
	}

	void Initialize();
	void Close();

	void WaitKey();
	BOOL IsAKeyPressed();
	uint8 _GetKeyScan();

	char GetFirstKey()
	{
		return _SymMap[_GetKeyScan()];
	}
	

private:

	void _InstallIntHandler();
	void _UninstallIntHandler();
};

extern KEYINPUT KeyInput;


#define Pressed(Key) KeyInput._KeyMap[Key]


} // namespace Input
} // namespace KInput


#endif // #ifndef _KEYINPUT_H_
