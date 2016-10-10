/*
 *  audioserver.cpp
 *
 *  Created by Tom on 14/09/2008.
 *
 */


#include "audioserver.h"
#include <iostream>

#if WIN32 
#define snprintf sprintf_s 
#endif

class AserveConnection :	public InterprocessConnection
{
	private:
		ThreadData threadData;
		
	public:
	AserveConnection() : InterprocessConnection (false)
	{
        initialiseThreadData();
        connect();
        std::cout << "For use with Aserve v1.4" << std::endl;
		std::cout.flush();
	}

	~AserveConnection()
	{
		//shutdownJuce_NonGUI();		//this probbaly shouldn't be in a juce class 
	}								//but I'm not sure where else to put it

	void connectionMade()
	{
		std::cout << "Aserver Connection Started\n" << std::endl;
	}

	void connectionLost()
	{
		std::cout << "Aserver Connection Lost\n"<< std::endl;
	}

	void messageReceived (const MemoryBlock& message)
	{
		//std::cout << "Aserver Message Received: " << (const char *) message.toString() << std::endl;

		if(message.toString().startsWith("midiraw"))		
		{
			void (*midiFuncPtr)(int,int,int);
			
			MidiMsg midiMsg;
			String string(message.toString());
			string = string.substring(string.indexOfChar(':')+1);
			midiMsg.statusbyte = string.getIntValue();
			string = string.substring(string.indexOfChar(':')+1);
			midiMsg.databyte1 = string.getIntValue();
			string = string.substring(string.indexOfChar(':')+1);
			midiMsg.databyte2 = string.getIntValue();
			
			//decode the message and set all variables in threddata
			threadData.access.enter();
			if( (midiMsg.statusbyte & 0xf0) == 0x90 || (midiMsg.statusbyte & 0xf0) == 0x80) //note
			{
				midiMsg.ticks = (long)Time::getHighResolutionTicks();
				MidiMsg temp = midiMsg;
				temp.statusbyte = midiMsg.statusbyte & 0x0f;//strip off the message nibble
				threadData.note.add(midiMsg);
				
				if(threadData.note.size() > 32)//if there are more than 32 notes in the queue then scrap it
				{
					threadData.note.clear();
				}
			}
			else if( (midiMsg.statusbyte & 0xf0) == 0xb0 )	//control
			{
				threadData.control[midiMsg.databyte1].statusbyte = midiMsg.statusbyte & 0x0f;
				threadData.control[midiMsg.databyte1].databyte1  = midiMsg.databyte1;
				threadData.control[midiMsg.databyte1].databyte2  = midiMsg.databyte2;
			}
			else if( (midiMsg.statusbyte & 0xf0) == 0xc0 )	//program
			{
				threadData.program.statusbyte = midiMsg.statusbyte & 0x0f;
				threadData.program.databyte1  = midiMsg.databyte1;
				threadData.program.databyte2  = midiMsg.databyte2;
			}
			else if( (midiMsg.statusbyte & 0xf0) == 0xd0 )	//aftertouch
			{
				threadData.afterTouch.statusbyte = midiMsg.statusbyte & 0x0f;
				threadData.afterTouch.databyte1  = midiMsg.databyte1;
				threadData.afterTouch.databyte2  = midiMsg.databyte2;
			}
			else if( (midiMsg.statusbyte & 0xf0) == 0xe0 )	//pitchbend
			{
				threadData.pitchBend.statusbyte = midiMsg.statusbyte & 0x0f;
				threadData.pitchBend.databyte1  = midiMsg.databyte1;
				threadData.pitchBend.databyte2  = midiMsg.databyte2;
			}
			threadData.access.exit();
			
			//------------------------------------------------------------------
			//check for the midimessage callback if it has been registered-----
			threadData.access.enter();
			midiFuncPtr = threadData.midiFunc;
			threadData.access.exit();	
			if(midiFuncPtr)			//if a midi calback has been registered then 
			{							//decode the bytes and call it.
				//call the callback
				(*midiFuncPtr)(midiMsg.statusbyte,midiMsg.databyte1,midiMsg.databyte2);
			}	
			
			threadData.access.enter();
			if((threadData.waitMidiMessage.statusbyte & 0xf0) //so we don't do it next time 
			   && ((midiMsg.statusbyte & 0xf0) == threadData.waitMidiMessage.statusbyte)) 
			{								
				threadData.waitMidiMessage.statusbyte = midiMsg.statusbyte & 0x0f;	 
				threadData.waitMidiMessage.databyte1 = midiMsg.databyte1;			
				threadData.waitMidiMessage.databyte2 = midiMsg.databyte2;
				threadData.incoming.signal();
			}	
			threadData.access.exit();
		}
		else if(message.toString().startsWith("sequence"))		
		{
			String string(message.toString());
			string = string.substring(string.indexOfChar(':')+1);
			int row = string.getIntValue();
			string = string.substring(string.indexOfChar(':')+1);
			int value = string.getIntValue();
			
			threadData.access.enter();
			threadData.sequenceBits.set(row, value);
			threadData.access.exit();
		}

		//----------------------------------------------------------------
		//Check to see if the other thread is waiting for a specific message
		threadData.access.enter();
		if(threadData.waitKeyword && message.toString().startsWith(String(threadData.waitKeyword)))
		{
			//must free this later - in the other thread
			//asprintf(&threadData.responce_str, "%s", (const char *)message.toString());
			threadData.responceStr = (char *)malloc(message.toString().length()+1);//+1 for the null
            printf("YESSSS!!!");
			snprintf(threadData.responceStr, message.toString().length()+1,"%s", (const char *)message.toString().toWideCharPointer());
			threadData.waitKeyword = NULL; // so we don't do it next time 
			//signal to the other thread
			threadData.incoming.signal();
		}	
		threadData.access.exit();
		//----------------------------------------------------------------
		//Update thread data 
	}	

	bool connect()
	{
		String host("127.0.0.1");
		bool openedOk = false;
		openedOk = connectToSocket (host,8000,1000);
		
		if (! openedOk)
		{
			std::cout << "Failed to connect - check Aserve status..."<< std::endl;
		}
		return openedOk;
	}

	void deliverMessage(const char *msg)
	{	
		const String text (msg);
		MemoryBlock messageData (text.toUTF8(), text.length());
		
		if (!sendMessage (messageData))
		{
			// the write failed, so indicate that the connection has broken..
			//std::cout << "send message failed! - " << msg << std::endl;
		}
	}
		
	void retrieveMessage(const char *requestMessage,char *responceMessage) 
	{
		//wait for next message beginning with requestMessage
		
		//lock the thread as this function will be called by the main thread
		threadData.access.enter();
		threadData.waitKeyword = (char *)requestMessage;//copy keyword so that the comms thread can look for it.
		
		//wait for message to be recieved
		threadData.access.exit();
		threadData.incoming.wait();
		
		//the responce message can now be processed - take a copy and free and unlock
		threadData.access.enter();
		snprintf(responceMessage,MAX_STRING_LEN,"%s",threadData.responceStr);
		//strcpy(responceMessage,threadData.responceStr);
		free(threadData.responceStr);
		threadData.responceStr = NULL;
		threadData.access.exit();
		//std::cout << "responceMessage" << responceMessage << std::endl;
	}
	
	void retrieveMidiMessage(int requestType, MidiMsg& midiMsg) 
	{
		//wait for next message beginning with requestMessage
		
		//lock the thread as this function will be called by the main thread
		threadData.access.enter();
		threadData.waitMidiMessage.statusbyte = requestType;//copy message type so that the comms thread can look for it.
		
		//wait for message to be recieved
		threadData.access.exit();
		threadData.incoming.wait();
		
		//the responce message can now be processed - take a copy and unlock
		threadData.access.enter();
		midiMsg.statusbyte = threadData.waitMidiMessage.statusbyte;
		midiMsg.databyte1  = threadData.waitMidiMessage.databyte1;
		midiMsg.databyte2  = threadData.waitMidiMessage.databyte2;
		threadData.waitMidiMessage.statusbyte = threadData.waitMidiMessage.databyte1 = threadData.waitMidiMessage.databyte2 = 0;
		threadData.access.exit();
	}
	
	void retrieveNoteMessage(MidiMsg& midiMsg) 
	{
		//Messages should be fed into a vector with a timestamp - if the timestamp exceeds 5ms then 
		long ticksNow = (long)Time::getHighResolutionTicks();
		threadData.access.enter();
		while(threadData.note.size())//if there are any on the note buffer then skim them off
		{
			midiMsg = threadData.note.remove(0);
			threadData.lastNote = midiMsg;
			
			if(Time::highResolutionTicksToSeconds(ticksNow - midiMsg.ticks) < 0.01)//only if they're not too old
			{
				threadData.access.exit();
				return;
			}
		}
		//wait for next message beginning with requestMessage
		threadData.waitMidiMessage.statusbyte = 0x90;//so that the comms thread can look for note
		
		//wait for message to be recieved
		threadData.access.exit();
		threadData.incoming.wait();
		
		//the responce message can now be processed - take a copy and unlock
		threadData.access.enter();
		midiMsg = threadData.note.remove(0);
		threadData.lastNote = midiMsg;
		threadData.waitMidiMessage.statusbyte = threadData.waitMidiMessage.databyte1 = threadData.waitMidiMessage.databyte2 = 0;
		threadData.access.exit();
		
	}
		
	void initialiseThreadData()
	{
		 resetThreadData();
	}
		
	void resetThreadData()
	{
		threadData.access.enter();
		threadData.note.clear();
		threadData.lastNote.statusbyte = threadData.lastNote.databyte1 = threadData.lastNote.databyte2 = 0;
		for(int i = 127; --i>=0;)
			threadData.control[i].statusbyte = threadData.control[i].databyte1 = threadData.control[i].databyte2 = 0;
		threadData.program.statusbyte = threadData.program.databyte1 = threadData.program.databyte2 = 0;
		threadData.afterTouch.statusbyte = threadData.afterTouch.databyte1 = threadData.afterTouch.databyte2 = 0;
		threadData.pitchBend.statusbyte = threadData.pitchBend.databyte1 = threadData.pitchBend.databyte2 = 0;
		for(int i = 0; i < 8;i++)
			threadData.sequenceBits.set(i,0);
		
		threadData.waitMidiMessage.statusbyte = threadData.waitMidiMessage.databyte1 = threadData.waitMidiMessage.databyte2 = 0;
		threadData.midiFunc = NULL;
		threadData.waitKeyword = NULL;
		threadData.access.exit();
	}


	void setMidiCallBack(void (*f)(int,int,int))
	{
		threadData.access.enter();
		threadData.midiFunc = f;
		threadData.access.exit();
	}
	
	MidiMsg getLastNote(void)
	{
		threadData.access.enter();
		const MidiMsg note = threadData.lastNote;
		threadData.access.exit();
		return note;
	}
	
	MidiMsg getControl(int number)
	{
		threadData.access.enter();
		const MidiMsg message = threadData.control[number];
		threadData.access.exit();
		return message;
	}
	
	MidiMsg getProgram(void)
	{
		threadData.access.enter();
		const MidiMsg message = threadData.program;
		threadData.access.exit();
		return message;
	}
	
	MidiMsg getAfterTouch(void)
	{
		threadData.access.enter();
		const MidiMsg message = threadData.afterTouch;
		threadData.access.exit();
		return message;
	}
	
	MidiMsg getPitchBend(void)
	{
		threadData.access.enter();
		const MidiMsg message = threadData.pitchBend;
		threadData.access.exit();
		return message;
	}
	
	int getSequenceRow(int row)
	{
		threadData.access.enter();
		const int value = threadData.sequenceBits[row];
		threadData.access.exit();
		return value;
	}
};

//----------------------------------------------------------------------------
//C stile wrapper for class access functions

AserveConnection* getConnection()
{	
    static AserveConnection ac;
	//connection has failed - clear up and prepare to re-open below
	if(ac.isConnected() == false)
		ac.connect(); //if this fails a message will have already been printed
									 //reset everything - no need to delete the object?
	return &ac;
}

void sendMessage(const char *msg)
{
	AserveConnection* asp = NULL;
	asp = getConnection();
	
	if(asp)//
		asp->deliverMessage(msg);
}

void retrieveMessage(const char *requestMessage, char *responceMessage)
{
	AserveConnection* asp = NULL;
	asp = getConnection();
	
	if(asp)//
		asp->retrieveMessage(requestMessage,responceMessage);	
}

void retrieveMidiMessage(int requestType, MidiMsg& midiMsg)
{
	AserveConnection* asp = NULL;
	asp = getConnection();
	
	if(asp)//
		asp->retrieveMidiMessage(requestType, midiMsg);	
}

void retrieveNoteMessage(MidiMsg& midiMsg)
{
	AserveConnection* asp = NULL;
	asp = getConnection();
	
	if(asp)//
		asp->retrieveNoteMessage(midiMsg);	
}



void setMidiCallback(void (*f)(int,int,int))
{
	AserveConnection* asp = NULL;
	asp = getConnection();
	
	if(asp && f)//
		asp->setMidiCallBack(f);	
}

MidiMsg getNote(void)
{
	AserveConnection* asp = NULL;
	MidiMsg note = {0,0,0};
	
	asp = getConnection();
	
	if(asp)//
		note = asp->getLastNote();
	
	return note;
}


MidiMsg getControl(int number)
{
	AserveConnection* asp = NULL;
	MidiMsg control = {0,0,0};
	
	asp = getConnection();
	
	if(asp)//
		control = asp->getControl(number);
	
	return control;
}

MidiMsg getProgram(void)
{
	AserveConnection* asp = NULL;
	MidiMsg program = {0,0,0};
	
	asp = getConnection();
	
	if(asp)//
		program = asp->getProgram();
	
	return program;
}

MidiMsg getAfterTouch(void)
{
	AserveConnection* asp = NULL;
	MidiMsg afterTouch = {0,0,0};
	
	asp = getConnection();
	
	if(asp)//
		afterTouch = asp->getAfterTouch();
	
	return afterTouch;
}

MidiMsg getPitchBend(void)
{
	AserveConnection* asp = NULL;
	MidiMsg pitchBend = {0,0,0};
	
	asp = getConnection();
	
	if(asp)//
		pitchBend = asp->getPitchBend();
	
	return pitchBend;
}

int getSequenceRow(int row)
{
	AserveConnection* asp = NULL;
	int value = 0;
	
	asp = getConnection();
	
	if(asp)//
		value = asp->getSequenceRow(row);
	
	return value;		
}

void ensureConnection(void)
{
	AserveConnection* asp = NULL;
	asp = getConnection();
}

