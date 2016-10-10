/*
 *  BitwiseSelector.cpp
 *  Aserve
 *
 *  Created by tjmitche on 15/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BitwiseSelector.h"

BitwiseSelector::BitwiseSelector()
{
    for (int i = 0; i < NumBits; ++i) 
    {
        bits.add (new TextButton());
        bits[i]->addListener (this);
        bits[i]->setColour (TextButton::buttonColourId, Colours::white);
        bits[i]->setColour (TextButton::buttonOnColourId, Colours::grey);
        bits[i]->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnLeft);
        addAndMakeVisible(bits[i]);
    }
}

BitwiseSelector::~BitwiseSelector()
{

}


void BitwiseSelector::setValue(int newValue)
{
    postMessage(new BitwiseMessage(newValue));
}

//Listeners
void BitwiseSelector::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void BitwiseSelector::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}


//Callbacks
void BitwiseSelector::resized()
{
    float x_frac = 1.f/NumBits;
    
    for (int i = 0; i < NumBits; ++i) 
    {
        bits[NumBits-1 - i]->setBoundsRelative(x_frac*i, 0.f, x_frac, 1.f);
    }
}

void BitwiseSelector::buttonClicked(Button* button)
{
    for (int i = 0; i < NumBits; ++i) 
    {
        if (bits[i] == button) 
        {
            listeners.call(&Listener::bitwiseSelectorClicked, this, i);
        }
    }
}

void BitwiseSelector::handleMessage (const Message& message)
{
    const BitwiseMessage& bitMessage = dynamic_cast<const BitwiseMessage&> (message);
    BigInteger bitsValue(bitMessage.bitsValue);
    for (int bit = 0; bit < NumBits; ++bit) 
    {
        bits[bit]->setToggleState(bitsValue[bit], dontSendNotification);
    }
}
