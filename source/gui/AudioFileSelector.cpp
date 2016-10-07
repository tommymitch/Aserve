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
    
    
    
    playButton = new TextButton ("u", "click here to play the current audio file");
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
    fileChooser->setBrowseButtonText ("...");
}

AudioFileSelector::~AudioFileSelector()
{
    playButton->removeListener (this);
    fileChooser->removeListener (this);
}

void AudioFileSelector::setPlayState (const int playState)
{
    postMessage(new AudioFileMessage(PlayMessage, playState));
}

void AudioFileSelector::setFileName (const String& path)
{
    postMessage(new AudioFileMessage(FileMessage, false, path));
}

//Listeners
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

void AudioFileSelector::handleMessage (const Message& message)
{
    const AudioFileMessage& afMessage = dynamic_cast<const AudioFileMessage&> (message);
    if (afMessage.type == PlayMessage) 
    {
        int playState = afMessage.playState;
        if (playState == -1) 
        {
            playButton->setEnabled (false);
        }
        else 
        {
            playButton->setEnabled (true);
            playButton->setToggleState(static_cast<bool> (playState), dontSendNotification);
        }
    }
    else if(afMessage.type == FileMessage)
    {
        fileChooser->setCurrentFile (File(afMessage.path), false, dontSendNotification);
    }
}