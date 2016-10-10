/*
  ==============================================================================

    Audio.cpp
    Created: 10 Oct 2016 10:35:54am
    Author:  Tom Mitchell

  ==============================================================================
*/

#include "Audio.h"


Audio::Audio()
{
    addMidiInputCallback (String::empty, this);
    addAudioCallback (this);
    initialise (0, 2, nullptr, false);
    
    oscReceiver.addListener (this);
    if (oscReceiver.connect (8000) == false)
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Connection Error", "Failled to connect to port 8000");
}

Audio::~Audio()
{
    oscReceiver.disconnect();
    oscReceiver.removeListener (this);
    
    closeAudioDevice();
    removeAudioCallback (this);
    removeMidiInputCallback (String::empty, this);
    
}

void Audio::audioDeviceIOCallback (const float** inputChannelData,
                                   int numInputChannels,
                                   float** outputChannelData,
                                   int numOutputChannels,
                                   int numSamples)
{
    
}

void Audio::audioDeviceAboutToStart (AudioIODevice* device)
{
    
}

void Audio::audioDeviceStopped()
{
    
}

void Audio::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    
}

void Audio::oscMessageReceived (const OSCMessage& message)
{
    
}