


#include "Logging.h"


namespace Logging
{


// class LOG

void LOG::Initialize(char Filename[])
{
	Close();

	File=fopen(Filename, "w");

	if(File==NULL)
		printf("Warning! Could not start log!\n");
	else
	{
		fprintf(File, "\n\n");
		Message("-------------- Log Started -------------");
	}
}

void LOG::Close()
{
	if(File!=NULL)
	{
		Message("--------------- Log Ended --------------");

		fclose(File);

		delete File;
		File=NULL;
	}
}

void LOG::Message(const char Msg[])
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s\n", time(NULL), *(unsigned long *)(0x0000046C), Msg);
}

void LOG::Message(const char Msg1[], const char Msg2[])
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s%s\n", time(NULL), *(unsigned long *)(0x0000046C), Msg1, Msg2);
}

void LOG::Message(const char Msg[], const long Val)
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s%i\n", time(NULL), *(unsigned long *)(0x0000046C), Msg, Val);
}

void LOG::Message(const char Msg[], const float Val)
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s%f\n", time(NULL), *(unsigned long *)(0x0000046C), Msg, Val);
}

void LOG::Message(const char Msg1[], const char Msg2[], const char Msg3[])
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s%s%s\n", time(NULL), *(unsigned long *)(0x0000046C), Msg1, Msg2, Msg3);
}

void LOG::Message(const char Msg1[], const char Msg2[], const char Msg3[], const char Msg4[])
{
	if(File!=NULL)
		fprintf(File, "%i : %i  -  %s%s%s%s\n", time(NULL), *(unsigned long *)(0x0000046C), Msg1, Msg2, Msg3, Msg4);
}

LOG Log;


} // namespace Logging
