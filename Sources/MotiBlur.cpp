


#include "MotiBlur.h"


namespace Graphics
{
namespace Graphics2D
{


// class MOTIONBLUR

void MOTIONBLUR::Initialize(uint8 Buffers, SURFACE *Target)
{	
	if(Buffers==0||Buffers>3)
	{
		cout<<"Error trying to initialize "<<(unsigned short)Buffers<<" Motion Blur Buffers."<<endl;
		exit(1);
	}
	
	Close();
	
	ActBuffer=0;
	(*this).Buffers=Buffers;
	(*this).Target=Target;

	BufferChain=(RGB **)Heap.Allocate(Buffers, sizeof(RGB *));

	for(uint8 i=0;i<Buffers;i++)
	{
		BufferChain[i]=(RGB *)Heap.PAllocate(Target->MaxT, sizeof(RGB));
		memset(BufferChain[i], 0x00, Target->MaxT);
	}

	Log.Message("Created Motion Blur object.");
}

void MOTIONBLUR::Close()
{
	if(BufferChain!=NULL)
	{
		for(uint8 i=0;i<Buffers;i++)
			Heap.Free(BufferChain[i]);

		Heap.Free(BufferChain);
		BufferChain=NULL;

		Log.Message("Closed Motion Blur object.");
	}

	Target=NULL;
	Buffers=0;
}

void MOTIONBLUR::Do()
{
	// Swap Target->Data with the "first" MotionBlur Buffer
	RGB *auxp;
	auxp=Target->Data;
	Target->Data=BufferChain[ActBuffer];
	BufferChain[ActBuffer]=auxp;

	// Move on the Buffer chain
	ActBuffer++;
	if(ActBuffer>=Buffers)
		ActBuffer=0;

	unsigned short i;
	uint8 k;

	switch(Buffers)
	{
		case(0):
		{
			break;
		}
		
		case(1):
		{
			for(i=0;i<Target->MaxT;i++)
				Target->Data[i]=MBMix(ARGB(128, Target->Data[i]), BufferChain[0][i]);
				
			break;
		}
		
		case(2):
		{
			for(i=0;i<Target->MaxT;i++)
				Target->Data[i]=MBMix(ARGB(43, Target->Data[i]), MBMix(ARGB(128, BufferChain[ActBuffer][i]), BufferChain[!ActBuffer][i]));

			break;
		}
		
		case(3):
		{
			RGB *Src[4];
			
			Src[0]=Target->Data;
			
			Src[1]=BufferChain[ActBuffer];
			
			k=ActBuffer+1;
			if(k>=3)
				k=0;
			Src[2]=BufferChain[k];
			
			k++;
			if(k>=3)
				k=0;
			Src[3]=BufferChain[k];

			for(i=0;i<Target->MaxT;i++)
				Target->Data[i]=MBMix(ARGB(9, Src[0][i]), MBMix(ARGB(43, Src[1][i]), MBMix(ARGB(128, Src[2][i]), Src[3][i])));

			break;
		}
	}
}


} // namespace Graphics2D
} // namespace Graphics
