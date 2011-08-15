/*
 *  AserveAudio.h
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ASERVEAUDIO
#define H_ASERVEAUDIO

#include <juce.h>
#include "AudioFilePlayerManager.h"
#include "OscillatorManager.h"
#include "Scope.h"

/**
 Where all audio and midi computation is done.
 */

class AserveAudio : public AudioIODeviceCallback,
                    public MidiInputCallback,
                    public AudioFilePlayerManager::Listener
{
public:
    /**
     Get rid of these and use text in the command!
     */
    enum
    {
        PLAYFILE,
        LOADFILE,
        
        COMMAND_COUNT
    };
    
    /**
     Audio constructor
     */
    AserveAudio();
    
    /**
     Audio destructor
     */
    ~AserveAudio();
    
    /**
     Returns the device manager
     */
    AudioDeviceManager* getDeviceManager(){return &audioDeviceManager;}
    
    /**
    Sets the scope object so that audio is streamed to the object for display
     */
    void setScopeObject(Scope* scope) {scopeObject = scope;}
    
    /**
     Checks a paramater to see if it is in range - 
     this should be a more general function in a utilities class
     */
	bool inRange(const char *parameter, double value, double min, double max);
    
    /**
     Stops all oscillators and files from playing
     */
	void stopAll(void);
    
    /**
     plays or stops the specified audio file player - if it's playing it stops - if it's stopped it plays.
     */
    void audioFile(const int audioFileIndex);
    
    /**
     plays or stops the specified audio file player
     */
    void audioFile(const int audioFileIndex, bool shouldPlay);
    
    /**
     instructs the specified audio file player to load the file at the given path
     */
    void audioFile(const int audioFileIndex, const String &newFilePath);
    
    /**
     Commands parser for all audio commands
     */
    void parseMessage(const String& message);
    
    //Audio Callbacks
    virtual void audioDeviceAboutToStart (AudioIODevice* device);
    virtual void audioDeviceStopped();
    virtual void audioDeviceIOCallback (const float** inAudio,int numInChannels,float** outAudio,int numOutAudio,int numSamples);
    //Midi Callback
	virtual void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message);
    //AudioFilePlayerManager Callback
    virtual void audioFileNameChanged(const int audioFileIndex, const String &path);
    virtual void audioFilePlayStateChanged(const int audioFileIndex, const AudioFilePlayerManager::PlayState state);
private:
	AudioDeviceManager audioDeviceManager;  //audio device
    MixerAudioSource mixerSource;           //all audio objects will be mixed into this
    AudioSourcePlayer audioSourcePlayer;    //and then playback using this
    
    ScopedPointer <AudioFilePlayerManager> audioFiles;  //audio file playback
    ScopedPointer <OscillatorManager> oscillators;      //oscialltor audio
    
    Scope* scopeObject;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveAudio)
};  

#endif //H_ASERVEAUDIO