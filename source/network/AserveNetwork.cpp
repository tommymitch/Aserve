/*
 *  AserveNetwork.cpp
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveNetwork.h"
#include "../controller/AserveController.h"


AserveNetwork::AserveNetwork()
{
    
}

AserveNetwork::~AserveNetwork()
{

}

//Callbacks 
void AserveNetwork::connected()
{
    AserveController* ac = AserveController::getInstance();
    if (ac) 
    {
        //std::cout << "con\n";
        ac->setNetworkConnectionParameter(true);
    }
}

void AserveNetwork::disconnected()
{
    AserveController* ac = AserveController::getInstance();
    if (ac) 
    {
        //std::cout << "dis\n";
        ac->reset();
    }
}

void AserveNetwork::messageReceived (const String& message)
{
    AserveController* ac = AserveController::getInstance();
    if (ac)
        ac->parseNetworkMessage(message);
}
