


#include "SB.h"


namespace SB
{


void interrupt _SB_IntHandler()
{
	SoundBlaster.CurrentBlock=!SoundBlaster.CurrentBlock;
	SoundBlaster._StartBlock();
	SoundBlaster._ManageSound();
	SoundBlaster._Acknowledge();
}


// struct SOUND

void SOUND::New(unsigned long Length)
{
	Delete();

	(*this).Length=Length;
	Data=(int8 *)Heap.Allocate(Length, sizeof(int8));
}

void SOUND::Delete()
{
	Stop();

	BOOL SendMsg=(Data!=NULL);
	Heap.Free(Data);
	Length=0;
	Reset();

	if(SendMsg)
		Log.Message("Deleted Sound.");
}

void SOUND::LoadSND(char Filename[])
{
	Delete();

	FILE *File=fopen(Filename, "rb");

	if(File==NULL)
	{
		Log.Message("Error opening file ", Filename);
		exit(1);
	}
	
	fread(&Length, sizeof(unsigned long) ,1 ,File);

	New(Length);
	fread(Data, sizeof(int8), Length, File);
	fclose(File);

	for(unsigned long i=0;i<Length;i++)
		Data[i]-=128;

	Log.Message("!FAIL! Loaded sound from file ", Filename);
}

void SOUND::LoadWAV(char Filename[])
{
	Delete();

	FILE *File;

	if((File=fopen(Filename,"rb"))==NULL)
	{
		Log.Message("!FAIL! Error opening file ", Filename);
		exit(1);
	}

	fskip(File, 54);
	
	fread(&Length, sizeof(unsigned long), 1, File);

	fskip(File, 3);

	New(Length);
	fread(Data, sizeof(uint8), Length, File);
	fclose(File);

	Log.Message("Loaded sound from wave file ", Filename);
}

void SOUND::Append(SOUND *Source)
{
	int8 *Data2=(int8 *)Heap.Allocate(Length+Source->Length, sizeof(int8));

	memcpy(Data2, Data, Length);
	memcpy(&Data2[Length], Source->Data, Source->Length);

	Heap.Free(Data);
	Data=Data2;
	Length+=Source->Length;
}


// Other

void _SB_Play(SOUND *Sound)
{
	SoundBlaster.Play(Sound);
}

void _SB_Stop()
{
	SoundBlaster.Stop();
}

void _SB_Pause()
{
	SoundBlaster.Pause();
}


// class SOUNDBLASTER

void SOUNDBLASTER::SetRate(unsigned long Rate)
{
	(*this).Rate=Rate;

	_disable(); // asm cli
	
	_WriteDSP(0x40);
	_WriteDSP(256-(uint8)((float)1000000/(float)Rate));

	_enable(); // asm sti
}

void SOUNDBLASTER::Initialize(unsigned long Rate)
{
	Close();

	(*this).Rate=Rate;
		
	if(!_GetEnv())
	{
		Log.Message("!FAIL! Error trying to get BLASTER Env.");
		Close();
		return;
	}

	Log.Message("Got BLASTER Env.");

	if(!_Initialize())
	{
		Log.Message("!FAIL! Unable to initialize Sound Blaster.");
		Close();
		return;
	}
		
	Log.Message("Created Sound Blaster device.");
}

void SOUNDBLASTER::Close()
{
	if(Buffer!=NULL)
	{
		_Close();

		Log.Message("Closed Sound Blaster device.");
	}
}

void SOUNDBLASTER::Play(SOUND *Sound)
{
	if(!IsCreated())
		return;

	_disable(); // asm cli

	if((*this).Sound!=NULL)
	{
		(*this).Sound->_Playing=0;
		(*this).Sound->Reset();
	}

	ClearBlock(0);
	ClearBlock(1);
	
	(*this).Sound=Sound;
	_RestartSound();

	_enable(); // asm sti
}

void SOUNDBLASTER::Stop()
{
	if(!IsCreated())
		return;

	_disable(); // asm cli
	
	if(Sound!=NULL)
	{
		Sound->_Playing=0;
		Sound->Reset();
		Sound=NULL;
	}

	ClearBlock(0);
	ClearBlock(1);

	_SB_IntHandler();

	_enable(); // asm sti
}

void SOUNDBLASTER::Pause()
{
	if(!IsCreated())
		return;

	_disable(); // asm cli

	if(Sound!=NULL)
	{
		Sound->_Playing=0;
		Sound=NULL;
	}

	ClearBlock(0);
	ClearBlock(1);

	_SB_IntHandler();

	_enable(); // asm sti
}

void SOUNDBLASTER::_StartBlock()
{
	outp(DMAMaskPort, DMAStopMask);
	outp(DMAClrPtrPort, 0x00);
	outp(DMAModePort, DMAMode);
	outp(DMAAddrPort, (uint8)((unsigned long)Buffer[CurrentBlock]&0x000000ff));
	outp(DMAAddrPort, (uint8)(((unsigned long)Buffer[CurrentBlock]&0x0000ff00)>>8));
	outp(DMAPagePort, (uint8)((unsigned long)Buffer[CurrentBlock]>>16));
	outp(DMACountPort, (uint8)((_BlockLength-1)&0x00ff));
	outp(DMACountPort, (uint8)(((_BlockLength-1)&0xff00)>>8));
	outp(DMAMaskPort, DMAStartMask);
	
	_WriteDSP(0x14);
	_WriteDSP((uint8)((_BlockLength-1)&0x00ff));
	_WriteDSP((uint8)(((_BlockLength-1)&0xff00)>>8));
}

BOOL SOUNDBLASTER::_GetEnv()
{
	char Blaster[128];
	char *Env=getenv("BLASTER");

	if(Env==NULL)
		return 0;

	strcpy(Blaster, Env);
	strupr(Blaster);

	if (!_GetTok(Blaster, 'A', 1, &BaseIO))
		return 0;
		
	if (!_GetTok(Blaster, 'I', 0, &IRQ))
		return 0;
		
	if (!_GetTok(Blaster, 'D', 0, &DMA))
		return 0;

	return 1;
}

BOOL SOUNDBLASTER::_GetTok(char *String, char ID, BOOL Hex, short *Value)
{
	char *ParamStart;
	char Buf1[128];
	char Buf2[128];

	strcpy(Buf1, String);
	
	if(strchr(Buf1, ID)==NULL)
		return 0;
		
	ParamStart=strchr(Buf1, ID)+1;

	if (strchr(ParamStart, ' ')!=NULL)
		*(strchr(ParamStart, ' '))='\0';

	if (Hex)
		strcpy(Buf2, "0x");
	else
		strcpy(Buf2, "");

	strcat(Buf2, ParamStart);

	*Value=(short)strtoul(Buf2, NULL, 0);

	return 1;
}

BOOL SOUNDBLASTER::_Initialize()
{
	// Set the IO ports
	ResetPort=BaseIO+0x006;
	ReadPort=BaseIO+0x00a;
	WritePort=BaseIO+0x00c;
	PollPort=BaseIO+0x00e;
	AckPort=BaseIO+0x00e;
	
	if(!_ResetDSP())
		return 0;
	
	_WriteDSP(0xe1);
	DSPVersion=_ReadDSP()+_ReadDSP()/100.;
	
	// Calculate interrupt controller ports and parameters
	if(IRQ<8) // PIC 1
	{
		IRQIntVector=0x08+IRQ;
		PICRotatePort=0x20;
		PICMaskPort=0x21;
	}
	else // PIC 2
	{
		IRQIntVector=0x70+IRQ-8;
		PICRotatePort=0xa0;
		PICMaskPort=0xa1;
	}
	IRQStopMask=(1<<(IRQ%8));
	IRQStartMask=~IRQStopMask;

	
	// Calculate DMA controller ports and parameters
	DMAMaskPort=0x0a;
	DMAClrPtrPort=0x0c;
	DMAModePort=0x0b;
	DMAAddrPort=(DMA<<1);
	DMACountPort=(DMA<<1)+0x01;
	
	switch(DMA)
	{
		case 0:
			DMAPagePort=0x87;
			break;
		case 1:
			DMAPagePort=0x83;
			break;
		case 2:
			DMAPagePort=0x81;
			break;
		case 3:
			DMAPagePort=0x82;
			break;
		default:
			return 0;
	}
	
	DMAStopMask=DMA+0x04;
	DMAStartMask=DMA+0x00;
	DMAMode=DMA+0x48;

	Buffer=(uint8 (*)[2])Heap.DOSAllocate(_BlockLength*(sizeof(uint8)*2), &BufferSel);
	ClearBlock(0);
	ClearBlock(1);

	Sound=NULL;

	_InstallHandler();

	_InitializeDAC();
	SetRate(Rate);
	
	return 1;
}

void SOUNDBLASTER::_Close()
{
	Stop();
	_UninstallHandler();
	_CloseDAC();
	_ResetDSP();
	
	Heap.DOSFree(BufferSel);
}

void SOUNDBLASTER::_WriteDSP(uint8 Value)
{
	while((inp(WritePort)&0x80));
	outp(WritePort, Value);
}

uint8 SOUNDBLASTER::_ReadDSP()
{
	while(!(inp(PollPort)&0x80));
	return (uint8)inp(ReadPort);
}

BOOL SOUNDBLASTER::_ResetDSP()
{
	outp(ResetPort, 1);
	delay(100);
	outp(ResetPort, 0);

	// Try 100 times
	for(uint8 i=0;i<100;i++)
		if(_ReadDSP()==0xaa)
			return 1;

	return 0;
}

void SOUNDBLASTER::_InstallHandler()
{
	_disable(); // asm cli
	
	outp(PICMaskPort, (inp(PICMaskPort)|IRQStopMask));
	
	OldIntHandler=_dos_getvect(IRQIntVector);
	_dos_setvect(IRQIntVector, _SB_IntHandler);
	
	outp(PICMaskPort, (inp(PICMaskPort)&IRQStartMask));
	
	_enable(); // asm sti

	Log.Message("Installed Sound Blaster interrupt handler.");
}
	
void SOUNDBLASTER::_UninstallHandler()
{
	_disable(); // asm cli
	
	outp(PICMaskPort, (inp(PICMaskPort)|IRQStopMask));
	
	_dos_setvect(IRQIntVector, OldIntHandler);
	
	_enable(); // asm sti

	Log.Message("Uninstalled Sound Blaster interrupt handler.");
}

void SOUNDBLASTER::_InitializeDAC()
{	  
	outp(DMAMaskPort, DMAStopMask);
	outp(DMAClrPtrPort, 0x00);
	outp(DMAModePort, DMAMode);
	outp(DMAAddrPort, (uint8)((unsigned long)Buffer&0x000000ff));
	outp(DMAAddrPort, (uint8)(((unsigned long)Buffer&0x0000ff00)>>8));
	outp(DMAPagePort, (uint8)((unsigned long)Buffer>>16));
	outp(DMACountPort, (uint8)((_BlockLength-1)&0x00ff));
	outp(DMACountPort, (uint8)(((_BlockLength-1)&0xff00)>>8));
	outp(DMAMaskPort, DMAStartMask);

	_WriteDSP(0xd1);
	_WriteDSP(0x14);
	_WriteDSP((uint8)((_BlockLength-1)&0x00ff));
	_WriteDSP((uint8)(((_BlockLength-1)&0xff00)>>8));

	Log.Message("Initialized Sound Blaster DAC.");
}

void SOUNDBLASTER::_CloseDAC()
{
	_WriteDSP(0xd0);
	_WriteDSP(0xd3);

	outp(DMAMaskPort, DMAStopMask);

	Log.Message("Closed Sound Blaster DAC.");
}

SOUNDBLASTER SoundBlaster;


} // namespace SB
