/*
 *  AudioFileSelector.cpp
 *  Aserve
 *
 *  Created by tjmitche on 12/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioFileSelector.h"

AudioFileSelector::AudioFileSelector()
{
    AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
    
    
    
    playButton = new PlayButton ("u", "click here to play the current audio file");
    playButton->addListener (this);
    playButton->setColour (TextButton::buttonColourId, Colours::white);
    playButton->setColour (TextButton::buttonOnColourId, Colours::grey);
    playButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnLeft);
    
    fileChooser = new FilenameComponent ("AudioFile",
                                         File::nonexistent,
                                         true, false, false,
                                         formatManager.getWildcardForAllFormats(),
                                         String::empty,
                                         "(choose an audio file)");
    
    addAndMakeVisible (playButton);
    addAndMakeVisible (fileChooser);
    
    fileChooser->addListener (this);
    fileChooser->setBrowseButtonText (T("..."));
}

AudioFileSelector::~AudioFileSelector()
{
    playButton->removeListener (this);
    fileChooser->removeListener (this);
}

void AudioFileSelector::setPlayState (const int playState)
{
    if (playState == -1) 
    {
        playButton->setEnabled (false);
    }
    else 
    {
        playButton->setEnabled (true);
        playButton->setToggleState(static_cast<bool> (playState), false);
    }
}

void AudioFileSelector::setFileName (const String& path)
{
    fileChooser->setCurrentFile (File(path), false, false);
}

void AudioFileSelector::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void AudioFileSelector::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}

//callbacks==========================================================================
void AudioFileSelector::resized()
{
    int x = getWidth();
    int y = getHeight();
    
    playButton->setBounds (0, 0, 30, y);
    fileChooser->setBounds (30, 0, x - 30, y);
}

void AudioFileSelector::filenameComponentChanged (FilenameComponent* filenameComponent)
{
    listeners.call(&Listener::audioFileNameChanged, this, filenameComponent->getCurrentFile().getFullPathName());
}

void AudioFileSelector::buttonClicked (Button* button)
{
    listeners.call(&Listener::audioFilePlayButtonClicked, this);
}