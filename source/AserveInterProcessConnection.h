/*
 *  AserveInterprocessConnection.h
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __ASERVEINTERPROCESSCONNECTION__
#define __ASERVEINTERPROCESSCONNECTION__

#include <juce.h>

class AserveInterprocessConnectionServer   :	public InterprocessConnectionServer,
												public ActionListener
	{
	private:
		OwnedArray <InterprocessConnection, CriticalSection> activeConnections;	//only use one of these but I'm not sure at the moment whether 
																				//the CriticalSection is needed. try to replace with just a pointer?
		
		ActionBroadcaster guiBroadcaster, audioBroadcaster, midiBroadcaster;
		int totalConnections;		//keeps track of the number of connections
	public:
		AserveInterprocessConnectionServer();
		~AserveInterprocessConnectionServer();
		InterprocessConnection* createConnectionObject();
		void actionListenerCallback(const String& message);
		void sendMessage(const String& text);
		void connectionMade();
		void connectionLost();
		
		void addGuiActionListener (ActionListener* const listener);
		void addAudioActionListener (ActionListener* const listener);
		void addMidiActionListener (ActionListener* const listener);
		
		void sendGuiActionMessage (const String& message); 
		void sendAudioActionMessage (const String& message);
		void sendMidiActionMessage (const String& message);

	};

class AserveInterprocessConnection  :	public InterprocessConnection
	
	{
	private:
		AserveInterprocessConnectionServer& owner;

	public:
		AserveInterprocessConnection(AserveInterprocessConnectionServer& owner_);		//this is so that we can post ActionMessages from within
																						//the connection callbacks
		~AserveInterprocessConnection();
		
		void connectionMade();
		void connectionLost();
		void messageReceived (const MemoryBlock& message);
	};


#endif   // __ASERVEINTERPROCESSCONNECTION__