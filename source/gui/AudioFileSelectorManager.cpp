/*
 *  AudioFileSelectorManager.cpp
 *  Aserve
 *
 *  Created by tjmitche on 12/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioFileSelectorManager.h"

AudioFileSelectorManager::AudioFileSelectorManager()
{
    for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i) 
    {
        selectors.add(new AudioFileSelector());
        selectors[i]->addListener(this);
        addAndMakeVisible(selectors[i]);
    }
}

AudioFileSelectorManager::~AudioFileSelectorManager()
{
    for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i) 
    {
        selectors[i]->removeListener(this);
    }
}

void AudioFileSelectorManager::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void AudioFileSelectorManager::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}

//callbacks==========================================================================
void AudioFileSelectorManager::resized()
{
    int x = getWidth();
    int y = getHeight()/AudioFilePlayerManager::NumPlayers;
    for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i) 
    {
        selectors[i]->setBounds(2, (y*i)+2, x-4, y-4);
    }
}

void AudioFileSelectorManager::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.getType() == AudioFileSelectorManagerSettings::SectionName) 
    {
        int selectorIndex = property.toString().getTrailingIntValue();
        if (isPositiveAndBelow(selectorIndex, static_cast<int>(AudioFilePlayerManager::NumPlayers)))
        {
            if (property.toString().startsWith(AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::PlayState].toString()))
            {
                selectors[selectorIndex]->setPlayState(treeWhosePropertyHasChanged.getProperty(property));
            }
            else if (property.toString().startsWith(AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::FileName].toString()))
            {
                selectors[selectorIndex]->setFileName(treeWhosePropertyHasChanged.getProperty(property));
            }
        }
        
//        {
//            int state = treeWhosePropertyHasChanged.getProperty(AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::PlayState]);
//            std::cout
//        }
//        else if (property == GuiSettings::Names[GuiSettings::BitRepresentationVisible]) 
//        {
//            bitwiseVisible = treeWhosePropertyHasChanged.getProperty(GuiSettings::Names[GuiSettings::BitRepresentationVisible]);
//            //bitwise->setVisible(bitwiseVisible);
//        }
    }
}

void AudioFileSelectorManager::audioFileNameChanged(const AudioFileSelector *selector, const String &fileName)
{
    for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i) 
    {
        if (selectors[i] == selector) 
        {
            listeners.call(&Listener::audioFileNameChanged, i, fileName);
        }
    }    
}

void AudioFileSelectorManager::audioFilePlayButtonClicked(const AudioFileSelector *selector)
{
    for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i) 
    {
        if (selectors[i] == selector) 
        {
            listeners.call(&Listener::audioFilePlayButtonClicked, i);
        }
    }
}