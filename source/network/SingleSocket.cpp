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
    if (start() == false)
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 PROJECT_NAME,"Unable to start server - port 8000 must be in use.\n if you're not sure what to do about it restart your machine and pray...");
	}
    else 
    {
        //NEED TO COMMUNICATE TO THE GUI THAT EVERYTHINGS GOOD
        //	if (openedOk)
        //		appendMessage (T("Waiting for aserve requests..."));
    }
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
    postMessage(new Message(Disconnected,0,0,0));//this has to happen on another thread otherwise we're deleting our own thread
}

void SingleSocket::handleMessage (const Message& message)
{
    if (message.intParameter1 == Disconnected) 
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
