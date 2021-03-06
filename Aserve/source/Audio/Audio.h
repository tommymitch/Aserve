/*
  ==============================================================================

    Audio.h
    Created: 10 Oct 2016 10:35:54am
    Author:  Tom Mitchell

  ==============================================================================
*/

#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

#include "OscillatorManager.h"

class Audio :   public  AudioDeviceManager,
                private AudioIODeviceCallback,
                private MidiInputCallback,
                private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
    Audio();
    ~Audio();
    
private:
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    void oscMessageReceived (const OSCMessage& message) override;
    
    OSCReceiver oscReceiver;
    OSCSender oscSender;
    
    AudioSourcePlayer audioSource;
    MixerAudioSource mixer;
    OscillatorManager oscillator;
    
    
};



#endif  // AUDIO_H_INCLUDED
