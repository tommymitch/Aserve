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
    
    mixer.addInputSource (&oscillator, false);
    audioSource.setSource (&mixer);
    
    initialise (0, 2, nullptr, false);
    
    oscReceiver.addListener (this);
    if (oscReceiver.connect (8000) == false)
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Connection Error", "Failled to connect to listen port 8000");
    
    if (oscSender.connect ("127.0.0.1", 8001) == false)
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Connection Error", "Failled to connect to send port 8001");
    
}

Audio::~Audio()
{
    oscReceiver.disconnect();
    oscReceiver.removeListener (this);
    
    audioSource.setSource (nullptr);
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
    audioSource.audioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}

void Audio::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSource.audioDeviceAboutToStart (device);
}

void Audio::audioDeviceStopped()
{
    audioSource.audioDeviceStopped();
}

void Audio::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    
}

void Audio::oscMessageReceived (const OSCMessage& message)
{
    DBG (message.getAddressPattern().toString());
    if (message.getAddressPattern().matches ({"/aserveOscillator"}))
    {
        const int index = message[0].getInt32();
        const float amplitude = message[2].getFloat32();
        if(amplitude > 0.f)
        {
            const float frequency = message[1].getFloat32();
            const int waveform = message[3].getInt32();
            oscillator.setFrequency (index, frequency);
            oscillator.setWave (index, waveform);
            oscillator.setAmplitude (index, amplitude);
        }
        else
        {
            oscillator.stop (index);
        }
    }
}