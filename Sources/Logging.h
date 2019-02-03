


#ifndef _LOGGING_H_
#define _LOGGING_H_


#include <stdio.h>
#include <time.h>
#include <stdlib.h>


namespace Logging
{


class LOG
{
public:

	FILE *File;

	LOG()
	{
		File=NULL;
	}

	~LOG() // !Warning! This might not destruct last! In order to insure it does, destruct all other by hand.
	{
		Close();
	}

	void Initialize(char Filename[]);
	void Close();

	void Message(const char Msg[]);
	void Message(const char Msg1[], const char Msg2[]);
	void Message(const char Msg[], const long Val);
	void Message(const char Msg[], const float Val);
	void Message(const char Msg1[], const char Msg2[], const char Msg3[]);
	void Message(const char Msg1[], const char Msg2[], const char Msg3[], const char Msg4[]);
};

extern LOG Log;


} // namespace Logging

using namespace Logging;


#endif // #ifndef _LOGGING_H_
