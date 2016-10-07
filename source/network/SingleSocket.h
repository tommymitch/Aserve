/*
 *  SingleSocket.h
 *  Aserve
 *
 *  Created by tjmitche on 07/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_SINGLESOCKET
#define H_SINGLESOCKET

#include "../../JuceLibraryCode/JuceHeader.h"

/**
Server for clients to connect to. Only one connection can be handled at a time.
*/
class SingleSocket :	public InterprocessConnectionServer,
                        public MessageListener
{
public:
    enum MessageCommands 
    {
        Disconnected,
        
        NumCommands
    };
    
    class SocketMessage : public Message
    {
    public:
        SocketMessage(const MessageCommands command_)  
        : command(command_) {}
        const MessageCommands command;
    };
    
    //==========================================================================
    SingleSocket();
    virtual ~SingleSocket();
    //==========================================================================
    
    /**
     Starts the server if this doesn't work it returns false other wise it starts a thread that 
     waits for a client to connect and returns true
     */
    bool start();
    
    /**
     called when the connection has been made
     */
    void connectionMade();
    
    /**
     called when the connection has been lost
     */
    void connectionLost();
    
    /**
     Function to send a message to the connected object.
     */
    void sendMessage(const String& text);
    
    /**
     Function called when the connection is made.
     */
    virtual void connected() {}
    /**
     Function called when the connection is broken.
     */
    virtual void disconnected() {}
    /**
     Function called when a message has been recived from the connected object.
     */
    virtual void messageReceived (const String& message) = 0;
    
    /**
     internal messages for transfering to the message thread
     */
    virtual void handleMessage (const Message& message);
    
private:
    //Callback
    virtual InterprocessConnection* createConnectionObject();
    
    ScopedPointer<InterprocessConnection> connection;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SingleSocket)
};

/**
 Class that is the network connection this is where all the work is done. Needs access to 
 the controller to envoke gui/audio/midi events
 */
class SocketConnection  :	public InterprocessConnection

{
    
public:
    SocketConnection(SingleSocket* owner_);	
    ~SocketConnection();
    
    //Callbacks
    virtual void connectionMade();
    virtual void connectionLost();
    virtual void messageReceived (const MemoryBlock& message);
    
private:
    SingleSocket* owner;
    
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SocketConnection)
};

#endif //H_SINGLESOCKET