


#include "Mouse.h"


namespace Input
{
namespace MInput
{

// class MOUSEINPUT

void MOUSEINPUT::Initialize()
{
	if(!Created)
	{
		union REGS regs;
			
		regs.w.ax=0x00;
		int386(0x33, &regs, &regs);
			
		if(!regs.w.ax)
		{
			cout<<"No mouse found."<<endl;
			exit(1);
		}
		
		Buttons[0]=Buttons[1]=0;
			
		Next();
		dx=dy=0;
		
		Created=1;

		Log.Message("Created Mouse Input device.");
	}
}

void MOUSEINPUT::Close()
{
	if(Created)
	{
		union REGS regs;
			
		regs.w.ax=0x00;
		int386(0x33, &regs, &regs);

		Created=0;

		Log.Message("Closed Mouse Input device.");
	}
}

MOUSEINPUT MouseInput;


} // MInput
} // Input
