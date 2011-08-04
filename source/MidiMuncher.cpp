/*
 *  MidiMuncher.cpp
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MidiMuncher.h"

MidiMuncher2::MidiMuncher2()
{
}

MidiMuncher2::~MidiMuncher2()
{
}

void MidiMuncher2::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message)
{
	
	unsigned char *rawData;	
	rawData = message.getRawData();
	
	String srawData(T("midiraw"));
	for(int i = 0; i < message.getRawDataSize();i++)
	{
		srawData = srawData + T(":") + String(*rawData);
		rawData++;
	}
	
	if(message.getRawDataSize() == 2)		//if only one databyte pad the other
		srawData = srawData + T(":") + String(-1);
	
	//printf("%s\n",(const char *)srawData);
	
	{
		//printf("MidiThread - %d (?) ActionMessageGenerated\n", Thread::getCurrentThreadId());
		sendActionMessage(srawData);	//this should broardcast the midi data to the InterProcessConnection object where it will be transmitted(hopefully)
		//const MessageManagerLock mmLock;//this locks the message thread try to do this through some other communications method change broardcaster.
		//owner.sendSocketMessage(srawData); -> need to post the data down the socket synchronously or as soon as possible
		//owner.appendMessage(srawData);
	}	
	
}
