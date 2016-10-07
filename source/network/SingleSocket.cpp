/*
 *  SingleSocket.cpp
 *  Aserve
 *
 *  Created by tjmitche on 07/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SingleSocket.h"

//Network server
SingleSocket::SingleSocket () : connection(NULL)
{
    
}

SingleSocket::~SingleSocket ()
{
    stop();
}

bool SingleSocket::start()
{
    stop();
	return beginWaitingForSocket (8000);	
}

void SingleSocket::connectionMade()
{
    if (connection != NULL) 
        connected();
}

void SingleSocket::connectionLost()
{
    if (connection != NULL) 
        disconnected();
    postMessage(new SocketMessage(Disconnected));//this has to happen on another thread otherwise we're deleting our own thread
}

void SingleSocket::handleMessage (const Message& message)
{
    const SocketMessage& sMessage = dynamic_cast<const SocketMessage&> (message);
    
    if (sMessage.command == Disconnected) 
    {
        connection = NULL;
    }
}

void SingleSocket::sendMessage(const String& text)
{
   if (connection != NULL) 
   {
       MemoryBlock messageData (text.toUTF8(), text.length());
       if(connection->sendMessage(messageData) == false)
       {
           //somethings gone wrong put up a message box
       }
   }
}

InterprocessConnection* SingleSocket::createConnectionObject()
{
    if (connection == NULL) 
    {
        connection = new SocketConnection (this);
    }
	else 
    {
        std::cout << "Already connected\n";
        //Show a messagebox "Already connected"
    }
    
	return connection;
}

//Network connection
SocketConnection::SocketConnection (SingleSocket* owner_) : 
                            InterprocessConnection (false), 
                            owner(owner_)
{
    
}

SocketConnection::~SocketConnection()
{
}

void SocketConnection::connectionMade()
{
	owner->connectionMade();
}

void SocketConnection::connectionLost()
{
    
	owner->connectionLost(); //let all other objects know that the connection was lost
}

void SocketConnection::messageReceived (const MemoryBlock& message)
{
    owner->messageReceived(message.toString());
}
