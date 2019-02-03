



#ifndef _MOUSE_H_
#define _MOUSE_H_


#include "General.h"


namespace Input
{
namespace MInput
{


#define MLeft   0
#define MRight  1


class MOUSEINPUT
{
public:
	BOOL Buttons[2];
	short dx;
	short dy;
	BOOL Created; // Do not modify this by hand

	MOUSEINPUT()
	{
		Created=0;
	}
	
	~MOUSEINPUT()
	{
		Close();
	}
	
	void Initialize();
	void Close();
	
	void Next()
	{
		union REGS regs;
	
		regs.w.ax=0x0b;
		int386(0x33, &regs, &regs);
		dx=regs.w.cx;
		dy=regs.w.dx;

		regs.w.ax=0x03;
		int386(0x33, &regs, &regs);
		Buttons[0]=regs.w.bx&0x01;
		Buttons[1]=regs.w.bx&0x02;
	}
};

extern MOUSEINPUT MouseInput;

#define Clicked(Button) MouseInput.Buttons[Button]


} // namespace Input
} // namespace MInput


#endif // #ifndef _MOUSE_H_

