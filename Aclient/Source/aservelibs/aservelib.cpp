/*
 *  aservelib.cpp
 *
 *  Created by Tom on 16/09/2008.
 *
 */

#include <stdio.h>
#include <iostream>

#include "aservelib.h"
#include "audioserver.h"

#if WIN32 
#define snprintf sprintf_s 
#endif

Aclient aclient;

void aserveOscillator (int oscillatorNumber, float frequency, float amplitude, int wave)
{
    aclient.oscMessageSend ({"/aserveOscillator", oscillatorNumber, frequency, amplitude, wave});
}

void aserveSleep(int milliseconds)
{
	Thread::sleep(milliseconds);
}

float aserveGetTime()
{
	static double startTime = Time::getMillisecondCounterHiRes();
	return (float)(Time::getMillisecondCounterHiRes() - startTime);
}

int aserveGetSequenceRow(int row)
{
	if(row >= 0 && row < 8)
		return getSequenceRow(row);
	else
		printf("Error: Sequencer row value %d is out of range (0 - 7)", row);
	
	return 0;
}

void aserveSetSequenceRow(int row, int value)
{
	char command[MAX_STRING_LEN];					
	snprintf(command,MAX_STRING_LEN,"g:b:%d:%d", row, value);
	//printf("command : %s", command);
	sendMessage(command);
}

int aserveGetNote()
{
	MidiMsg note = {0,0,0,0};

	retrieveNoteMessage(note);
	
	return note.databyte1;
}

/*int aserve WAIT FOR SOME TEXT STRING example()
{
	int note;
	char string[MAX_STRING_LEN];
	String jstring; //better than doing all that nasty array sheeet.
	
	recieveMessage("midinote", string);
	
	jstring = string;
	//decode the string
	jstring = jstring.substring(jstring.indexOfChar(':')+1);//first one is the channel
	jstring = jstring.substring(jstring.indexOfChar(':')+1);//second is the note
	note = jstring.getIntValue();
	
	return note;
}*/

int aserveGetVelocity(void)
{
	MidiMsg note = getNote();		
	return note.databyte2;
}

int aserveGetNoteChannel()
{
	MidiMsg note = getNote();	
	if(note.statusbyte == -1)
		return -1;//set when the bytes are in initial state
		
	return note.statusbyte & 0x0f;
}

int aserveGetControl(int number)
{
	MidiMsg control;
	control = getControl(number);
	return control.databyte2;
}

int aserveGetControlChannel(int number)
{
	MidiMsg control;
	control = getControl(number);
	if(control.statusbyte == -1)
		return -1;//set when the bytes are in initial state
	
	return control.statusbyte & 0x0f;
}

int aserveGetProgram(void)
{
	MidiMsg program;
	program = getProgram();
	return program.databyte1;
}

int aserveGetProgramChannel(void)
{
	MidiMsg program;
	program = getProgram();
	if(program.statusbyte == -1)
		return -1;//set when the bytes are in initial state
	
	return program.statusbyte & 0x0f;
}

int aserveGetAfterTouch(void)
{
	MidiMsg afterTouch;
	afterTouch = getAfterTouch();
	return afterTouch.databyte1;
}

int aserveGetAfterTouchChannel(void)
{
	MidiMsg afterTouch;
	afterTouch = getAfterTouch();
	if(afterTouch.statusbyte == -1)
		return -1;//set when the bytes are in initial state
	
	return afterTouch.statusbyte & 0x0f;
}

int aserveGetPitchBend(void)
{	
	MidiMsg pitchBend;
	pitchBend = getPitchBend();
	int combined = (pitchBend.databyte2 << 7) + pitchBend.databyte1;	
	if(combined < 0)
		combined = -1;//set when the bytes are in initial state
	return combined;
}

int aserveGetPitchBendChannel(void)
{	
	MidiMsg pitchBend;
	pitchBend = getPitchBend();
	if(pitchBend.statusbyte == -1)
		return -1;//set when the bytes are in initial state
	
	return pitchBend.statusbyte & 0x0f;
}

void aserveSetMidi(const int statusbyte, const int databyte1, const int databyte2)
{
	{
        char command[MAX_STRING_LEN];					
        snprintf(command,MAX_STRING_LEN,"a:m:%d:%d:%d", statusbyte, databyte1, databyte2);
        
        //printf("command : %s", command);
        sendMessage(command);
    }
}

//samp commands
//enum
//{
//	PLAYFILE,
//	LOADFILE,
//	
//	COMMAND_COUNT
//};

void aserveSample(int row, float amplitude)
{
	char command[MAX_STRING_LEN];					
	snprintf(command,MAX_STRING_LEN,"a:p:%d:%f", row, amplitude);//the zero is the play command (1 is the load command;)
	//printf("command : %s", command);
	sendMessage(command);
}

void aserveLoadSample(int row, const char *path)
{
	char command[MAX_STRING_LEN];					
	snprintf(command,MAX_STRING_LEN,"a:l:%d:%s", row, path);//the zero is the play command (1 is the load command;)
	//printf("command : %s", command);
	sendMessage(command);
}

void aserveSetMidiCallback(void (*f)(int,int,int))
{
	setMidiCallback(f);
}

#ifndef WIN32 
void aserveSay(const char *message)
{
	char *command = (char *)malloc(strlen(message)+10);
	strcpy(command, "say ");
	strcat(command, message);
	strcat(command, " &");
	system(command);
	free(command);
}
#endif//WIN32

