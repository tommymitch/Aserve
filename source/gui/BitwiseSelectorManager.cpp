/*
 *  BitwiseSelectorManager.cpp
 *  Aserve
 *
 *  Created by tjmitche on 15/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BitwiseSelectorManager.h"


BitwiseSelectorManager::BitwiseSelectorManager()
{
    for (int i = 0; i < NumSelectors; ++i) 
    {
        selectors.add(new BitwiseSelector());
        selectors[i]->addListener(this);
        addAndMakeVisible(selectors[i]);
    }
    
}

BitwiseSelectorManager::~BitwiseSelectorManager()
{
    for (int i = 0; i < NumSelectors; ++i) 
    {
        selectors[i]->removeListener(this);
    }
}

//Listeners
void BitwiseSelectorManager::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void BitwiseSelectorManager::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}


//Callbacks
void BitwiseSelectorManager::resized()
{
    float y_frac = 1.f/NumSelectors;
    
    for (int i = 0; i < NumSelectors; ++i) 
    {
        selectors[i]->setBoundsRelative(0.f, y_frac*i, 1.f, y_frac);
    }
}

void BitwiseSelectorManager::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.getType() == BitwiseSelectorManagerSettings::SectionName) 
    {
        int selectorIndex = property.toString().getTrailingIntValue();
        if (isPositiveAndBelow(selectorIndex, static_cast<int>(NumSelectors)) /*&& isPositiveAndBelow(x, static_cast<int>(BitwiseSelector::NumBits))*/) 
        {
            selectors[selectorIndex]->setValue(treeWhosePropertyHasChanged.getProperty(property));
        }
    }
}


void BitwiseSelectorManager::bitwiseSelectorClicked(const BitwiseSelector* selector, const int bitNumber)
{
    for (int selectorIndex = 0; selectorIndex < NumSelectors; ++selectorIndex) 
    {
        if (selectors[selectorIndex] == selector) 
        {
            listeners.call(&BitwiseSelectorManager::Listener::bitwiseSelectorClicked, bitNumber, selectorIndex);
        }
    }
}