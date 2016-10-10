/*
 *  audioserver.h
 *
 *  Created by Tom on 14/09/2008.
 *
 */


#ifndef __AUDIOSERVER_H__
#define __AUDIOSERVER_H__

#include "../../juceLibraryCode/JuceHeader.h"

class Aclient : private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
    Aclient();
    ~Aclient();
    
    void oscMessageSend (const OSCMessage& mes);
private:
    void oscMessageReceived (const OSCMessage& message) override;
    ScopedPointer<OSCReceiver> oscReciever;
    ScopedPointer<OSCSender> oscSender;
};



#define MAX_STRING_LEN 1024

//get rid of the above at some point and just use this
typedef struct
{
	int statusbyte;
	int databyte1;
	int databyte2;
	long ticks;		//time of arrival - only used for notes
}MidiMsg;

//to store all the last values
typedef struct
{
	MidiMsg note;
	MidiMsg control[128];
	MidiMsg program;
	MidiMsg afterTouch;
	MidiMsg pitchBend;
}MidiChannel;


typedef struct
{	
	WaitableEvent incoming;				//signal arrival of messages
	CriticalSection access;				//mutex style control
	
	Array <MidiMsg> note;
	MidiMsg lastNote;
	MidiMsg control[128];
	MidiMsg program;
	MidiMsg afterTouch;
	MidiMsg pitchBend;
	
	Array <int> sequenceBits;
	
	MidiMsg waitMidiMessage;
	char *waitKeyword;
	char *responceStr;
	void (*midiFunc)(int,int,int);//function callback for midi messages
	
}ThreadData;

void sendMessage(const char *msg);
void retrieveMessage(const char *requestMessage, char *responceMessage);
void retrieveMidiMessage(int requestType, MidiMsg& midiMsg);
void retrieveNoteMessage(MidiMsg& midiMsg);
void setMidiCallback(void (*f)(int,int,int));
MidiMsg getNote(void);
MidiMsg getControl(int number);
MidiMsg getProgram(void);
MidiMsg getAfterTouch(void);
MidiMsg getPitchBend(void);
int getSequenceRow(int row);
void ensureConnection(void);

#endif //__AUDIOSERVER_H__