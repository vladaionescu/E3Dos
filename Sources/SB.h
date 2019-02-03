


#ifndef _SB_H_
#define _SB_H_


#include "General.h"
#include "Heap.h"


#define _BlockLength 32768


namespace SB
{


struct SOUND
{
	BOOL _Playing;
	int8 *Data;
	unsigned long Length;
	unsigned long Position;

	SOUND *NextChunk;

	SOUND()
	{
		_Playing=0;
		Reset();
		Data=NULL;
		Length=0;
		NextChunk=NULL;
	}

	~SOUND()
	{
		Delete();
	}

	void New(unsigned long Length);
	void Delete();		

	BOOL IsCreated()
	{
		if(Data==NULL)
			return 0;

		return 1;
	}

	BOOL IsPlaying()
	{
		return _Playing;
	}

	void Reset()
	{
		Position=0;
	}

	void Play();
	void Stop();
	void Pause();

	void LoadSND(char Filename[]);
	void LoadWAV(char Filename[]);

	void Append(SOUND *Source);
};


void _SB_Play(SOUND *Sound);
void _SB_Stop();
void _SB_Pause();


inline void SOUND::Play()
{
	_SB_Play(this);
}

inline void SOUND::Stop()
{
	if(_Playing)
	{
		_Playing=0;
		_SB_Stop();
	}
}

inline void SOUND::Pause()
{
	if(_Playing)
		_SB_Pause();
	else
		_SB_Play(this);
}


void interrupt _SB_IntHandler();


class SOUNDBLASTER
{	
private:
	BOOL Created;
	
	short BaseIO;
	short IRQ;
	short DMA;
	
	short ResetPort;
	short ReadPort;
	short WritePort;
	short PollPort;
	short AckPort;

	short IRQIntVector;
	short PICRotatePort;
	short PICMaskPort;
	short IRQStopMask;
	short IRQStartMask;

	short DMAMaskPort;
	short DMAClrPtrPort;
	short DMAModePort;
	short DMAAddrPort;
	short DMACountPort;
	short DMAPagePort;
	short DMAStopMask;
	short DMAStartMask;
	short DMAMode;
	
	float DSPVersion;

	void interrupt (*OldIntHandler)();

	uint8 (*Buffer)[2];
	unsigned short BufferSel;
	unsigned long Rate;


public:

	SOUND *Sound;
	BOOL CurrentBlock;

	SOUNDBLASTER()
	{
		Buffer=NULL;
		Sound=NULL;
	}

	~SOUNDBLASTER()
	{
		Close();
	}

	BOOL IsCreated()
	{
		return (Buffer!=NULL);
	}

	BOOL IsPlaying()
	{
		return (Sound!=NULL);
	}

	unsigned long GetRate()
	{
		return Rate;
	}

	void SetRate(unsigned long Rate);

	void Initialize(unsigned long Rate);
	void Close();
	
	void Play(SOUND *Sound);
	void Stop();
	void Pause();

	void _StartBlock();

	void _ManageSound()
	{		
		if(Sound!=NULL)
			_ContinueSound();
		else
			ClearBlock(!CurrentBlock);
	}

	void _RestartSound()
	{		
		_disable(); // asm cli
		
		if(Sound->Length-Sound->Position<=_BlockLength)
		{
			memcpy((void *)Buffer[!CurrentBlock], Sound->Data+Sound->Position, Sound->Length-Sound->Position);
			Sound->_Playing=0;
			Sound->Reset();
			Sound=NULL;
			return;
		}

		Sound->_Playing=1;
		
		memcpy((void *)Buffer[!CurrentBlock], Sound->Data+Sound->Position, _BlockLength);
		Sound->Position+=_BlockLength;

		_SB_IntHandler();

		_enable(); // asm sti
	}

	void _Acknowledge()
	{
		inp(AckPort);		// Acknowledge interrupt with Sound Card
		outp(0xa0, 0x20);	// Acknowledge interrupt with PIC 2
		outp(0x20, 0x20);	// Akcnowledge interrupt with PIC 1
	}


private:

	void _ContinueSound()
	{		
		if(Sound->Length-Sound->Position<=_BlockLength)
		{
			ClearBlock(!CurrentBlock);
			memcpy((void *)Buffer[!CurrentBlock], (int8 *)(Sound->Data+Sound->Position), Sound->Length-Sound->Position);
			Sound->_Playing=0;

			if(Sound->NextChunk!=NULL) // !Warning! NextChunk should be bigger than Buffer[!CurrentBlock]
			{
				memcpy((void *)(Buffer[!CurrentBlock]+Sound->Length-Sound->Position), Sound->NextChunk->Data, _BlockLength-(Sound->Length-Sound->Position));
				Sound->NextChunk->Position=_BlockLength-(Sound->Length-Sound->Position);

				Sound->Reset();
				Sound=Sound->NextChunk;
			}
			else
			{
				Sound->Reset();
				Sound=NULL;
			}

			return;
		}

		memcpy((void *)Buffer[!CurrentBlock], (int8 *)(Sound->Data+Sound->Position), _BlockLength);
		Sound->Position+=_BlockLength;		
	}
	
	BOOL _GetEnv();
	BOOL _GetTok(char *String, char ID, BOOL Hex, short *Value);
	BOOL _Initialize();
	void _Close();

	void _WriteDSP(uint8 Value);
	uint8 _ReadDSP();
	BOOL _ResetDSP();

	void _InstallHandler();		
	void _UninstallHandler();

	void _InitializeDAC();
	void _CloseDAC();

	void ClearBlock(BOOL Block)
	{
		memset(Buffer[Block], 0x80, _BlockLength);
	}
};

extern SOUNDBLASTER SoundBlaster;


} // namespace SoundBlaster


#endif // #ifndef _SB_H_
