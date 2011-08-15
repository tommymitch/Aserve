/*
 *  AserveAudio.h
 *  Aserve
 *
 *  Created by tjmitche on 06/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __ASERVEAUDIO__
#define __ASERVEAUDIO__

#include <juce.h>
#include "AserveClasses.h"
#include "MidiMuncher.h" 
#include "AserveScope.h"

//samp commands
enum
{
	PLAYFILE,
	LOADFILE,
	
	COMMAND_COUNT
};


class OldAserveAudio : public Component,
					public AudioIODeviceCallback,
					public MidiInputCallback,
					public ChangeListener,
					public ActionBroadcaster,	//to deliver messages to the GUI
					public ActionListener		//to recieve messages from the interprocessconnection and from the gui
{
public:
	OldAserveAudio(AserveScope *aserveScopePtr);
	~OldAserveAudio();
	void audioDeviceIOCallback (const float** inputChannelData,
                                int totalNumInputChannels,
                                float** outputChannelData,
                                int totalNumOutputChannels,
                                int numSamples);
    
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
	void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message);
	void changeListenerCallback (ChangeBroadcaster* source);
	void addMidiActionListener(ActionListener* const listener);//To allow midi messages to trigger Interprocess action
	void actionListenerCallback(const String& message);//To respond to Interprocess commands from the client and gui
	
	//Non callbacks
	void parseMessage(const String& message);
	bool inRange(const char *parameter, double value, double min, double max);
	void stopAll(void);	//funciton to stop all audio - when client dissconnects for example
	void loadSample (int fileIndex, String& filePath);
	bool fileLoaded(int sampleNumber);
	bool filePlaying(int sampleNumber);
	void showAudioSettings(Component *centerComp);
	MidiMuncher2 *getMidiMuncher();
	
	
private:
	//==============================================================================

	// this wraps the actual audio device
	AudioDeviceManager audioDeviceManager;
	
	// this allows an audio source to be streamed to the IO device
	AudioSourcePlayer audioSourcePlayer;
	
	// this source contains our synth, and generates its output
	SynthAudioSource synthSource;
	
	//Bank of oscillators ready to go
	OscillatorBankAudioSource oscillatorSource;
	
	// this source contains a test tone
	//ToneGeneratorAudioSource toneSource;
	
	// this source is used to mix together the output from our synth source
	// and wave player source
	MixerAudioSource mixerSource;
	
	// this controls the playback of a positionable audio stream, handling the
	// starting/stopping and sample-rate conversion
	AudioTransportSource transportSource[8];									//put these in a class??with the ones below and make an Array?
	
	// this is the actual stream that's going to read from the audio file.
	AudioFormatReaderSource* currentAudioFileSource[8];
	File currentFile[8];
	
	//pointer to the scope in the AserveComponent object
	AserveScope *aserveScope; 
    
};


#endif   // __ASERVEAUDIO__