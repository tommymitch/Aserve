/*
 *  AserveNetwork.h
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ASERVENETWORK
#define H_ASERVENETWORK

#include "SingleSocket.h"

/**
 Class where all the network coms are managed this is where all links with the controller 
 should be manged
 */
class AserveNetwork : public SingleSocket
{
public:
    
    //This is a singleton class
    juce_DeclareSingleton(AserveNetwork, false);
    
    AserveNetwork();
    ~AserveNetwork();
    
    //Callbacks 
    virtual void connected();
    virtual void disconnected();
    virtual void messageReceived (const String& message);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveNetwork)
};

#endif //H_ASERVENETWORK