/*
 *  AserveNetwork.cpp
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveNetwork.h"
#include "AserveController.h"

juce_ImplementSingleton (AserveNetwork)

AserveNetwork::AserveNetwork()
{
    
}

AserveNetwork::~AserveNetwork()
{
    
    clearSingletonInstance();
}

//Callbacks 
void AserveNetwork::connected()
{
    std::cout << "connected\n";
}

void AserveNetwork::disconnected()
{
    std::cout << "disconnected\n";
    AserveController* ac = AserveController::getInstance();
    if (ac) 
        ac->reset();
}

void AserveNetwork::messageReceived (const String& message)
{
    AserveController* ac = AserveController::getInstance();
    if(ac)
        ac->parseNetworkMessage(message);
}
