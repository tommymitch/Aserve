/*
 *  AserveInterprocessConnection.cpp
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveInterProcessConnection.h"

/*-----------------------------------------------
 AserverInterprocessConnectionServer Implementation
 -----------------------------------------------*/

AserveInterprocessConnectionServer::AserveInterprocessConnectionServer () : totalConnections(0)
{
}

AserveInterprocessConnectionServer::~AserveInterprocessConnectionServer()
{
}

InterprocessConnection* AserveInterprocessConnectionServer::createConnectionObject()
{
	AserveInterprocessConnection* newConnection = new AserveInterprocessConnection (*this);
	//printf("We Have A Connection\n");
	activeConnections.add (newConnection);
	
	connectionMade();
	return newConnection;
}

void AserveInterprocessConnectionServer::actionListenerCallback(const String& message)
{
	//printf("ActionOrEventThread? - %u (?) ActionMessageConsumed\n", juce::Thread::getCurrentThreadId());
	//printf("AserveInterprocessConnectionServer Recieved ActionMessage::%s\n", (const char *)message);
	if(message.startsWith(T("seq")) ||  message.startsWith(T("midiraw")))
		sendMessage(message);
}

void AserveInterprocessConnectionServer::sendMessage(const String& text)
{
	MemoryBlock messageData (text.toUTF8(), text.length());
	
	for (int i = activeConnections.size(); --i >= 0;)		//problem here - need to clear activeConnections when concetion is lost - somehow
	{
		if (! activeConnections[i]->sendMessage (messageData))
		{
			// the write failed, so indicate that the connection has broken..
			sendGuiActionMessage(T("Socket Error: send message failed!"));
		}
	}		
}

void AserveInterprocessConnectionServer::connectionMade()
{
	totalConnections++;
	sendGuiActionMessage (T("con:1:") + String (totalConnections));
}

void AserveInterprocessConnectionServer::connectionLost()
{
	activeConnections.clear();		//delete the old connection if there was one.
	sendGuiActionMessage (T("con:0:") + String (totalConnections));
	sendAudioActionMessage(T("stop"));
}

void AserveInterprocessConnectionServer::addGuiActionListener (ActionListener* const listener)
{
	guiBroadcaster.addActionListener(listener);
}

void AserveInterprocessConnectionServer::addAudioActionListener (ActionListener* const listener)
{
	audioBroadcaster.addActionListener(listener);
}

void AserveInterprocessConnectionServer::addMidiActionListener (ActionListener* const listener)
{
	midiBroadcaster.addActionListener(listener);
}

void AserveInterprocessConnectionServer::sendGuiActionMessage (const String& message)
{
	guiBroadcaster.sendActionMessage(message);
}

void AserveInterprocessConnectionServer::sendAudioActionMessage (const String& message)
{
	audioBroadcaster.sendActionMessage(message);
}

void AserveInterprocessConnectionServer::sendMidiActionMessage (const String& message)
{
	midiBroadcaster.sendActionMessage(message);
}




/*-----------------------------------------------
 AserverInterprocessConnectionServer Implementation
 -----------------------------------------------*/

AserveInterprocessConnection::AserveInterprocessConnection (AserveInterprocessConnectionServer& owner_) : 
InterprocessConnection (true), 
owner(owner_)
{

}

AserveInterprocessConnection::~AserveInterprocessConnection()
{
}

void AserveInterprocessConnection::connectionMade()
{
	//all the work is done by createConnectionObject() in the server object
}

void AserveInterprocessConnection::connectionLost()
{
	owner.connectionLost(); //let all other objects know that the connection was lost
}

void AserveInterprocessConnection::messageReceived (const MemoryBlock& message)
{
	//owner.sendGuiActionMessage (T("Connection #") + String (ourNumber) + T(" - message received: ") + message.toString());
	//interperate message and respond		
	//printf("message recieved: %s\n",(const char *)message.toString());
	String string = message.toString();
	if(string.startsWith(T("sub")))
	{
		owner.sendGuiActionMessage(string);
	}
	else
	{
		owner.sendAudioActionMessage(string);
	}
}
