/*
 *  AudioFilePlayer.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioFilePlayer.h"

AudioFilePlayer::AudioFilePlayer() : currentAudioFileSource(0)
{
    transportSource.addChangeListener(this);
}


AudioFilePlayer::~AudioFilePlayer()
{
    transportSource.removeChangeListener(this); 
}

void AudioFilePlayer::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void AudioFilePlayer::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}


void AudioFilePlayer::loadAudioFile (const String& path)
{
	// this is called when the user changes the filename in the file chooser box
	bool error = false;
    File audioFile (path);
    
    // unload the previous file source and delete it..
    transportSource.stop ();
    transportSource.setSource (0);
    currentAudioFileSource = NULL;
    
	if(audioFile.existsAsFile())
	{
		// create a new file source from the file..
		// get a format manager and set it up with the basic types (wav and aiff).
		AudioFormatManager formatManager;
		formatManager.registerBasicFormats();
        
		AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
        
		if (reader != 0)
		{
			currentFile = audioFile;
			currentAudioFileSource = new AudioFormatReaderSource (reader, true);
            
			// ..and plug it into our transport source
			transportSource.setSource (currentAudioFileSource,
                                                  32768, // tells it to buffer this many samples ahead
                                                  reader->sampleRate);
		}
        else 
        {
            error = true;
        }

	}
    else 
    {
        error = true;
    }

    
	if (error) 
	{
        if (path != String::empty) 
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "Aserve: File Error",
                                         "It was not possible to load the file: \n" + audioFile.getFileName() + "\n Check the file path/format for problems.",
                                         "ok");
        }
		listeners.call(&Listener::audioFileNameChanged, this, String::empty);
        listeners.call(&Listener::audioFilePlayStateChanged, this, Disable);
	}
    else 
    {
        listeners.call(&Listener::audioFileNameChanged, this, audioFile.getFullPathName());
        listeners.call(&Listener::audioFilePlayStateChanged, this, Stop);
    }

}

void AudioFilePlayer::play(float gain)
{
    if (currentAudioFileSource != NULL)
    {
        transportSource.setGain(gain);
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void AudioFilePlayer::stop()
{
    if (currentAudioFileSource != NULL)
        transportSource.stop();
}

void AudioFilePlayer::togglePlayState()
{
    if (transportSource.isPlaying()) 
        stop();
    else 
        play();

}

//AudioSourceCallbacks
void AudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioFilePlayer::releaseResources()
{
    transportSource.releaseResources();
}

void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

//ChangeListener Callback
void AudioFilePlayer::changeListenerCallback (ChangeBroadcaster* source)
{
    listeners.call(&Listener::audioFilePlayStateChanged, this, static_cast<PlayState> (transportSource.isPlaying()));
    if(currentAudioFileSource == 0)
        listeners.call(&Listener::audioFilePlayStateChanged, this, Disable);
}
