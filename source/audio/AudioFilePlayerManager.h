/*
 *  AudioFilePlayers.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOFILEPLAYERMANAGER
#define H_AUDIOFILEPLAYERMANAGER

#include <juce.h>
#include "AudioFilePlayer.h"

/**
 Top level of the model class where all internal data is stored
 */
class AudioFilePlayerManager :  public AudioSource,
                                public AudioFilePlayer::Listener
{
public:
    enum AudioFilePlayerConstants 
    {
        NumPlayers = 8
    };
    /**
     Gui constructor
     */
    AudioFilePlayerManager();
    /**
     Gui destructor
     */
    ~AudioFilePlayerManager();
    
    /**
     Enum used in the callback
     */
    enum PlayState
    {
        Disable = AudioFilePlayer::Disable,
        Stop = AudioFilePlayer::Stop,
        Play = AudioFilePlayer::Play,
    };
    
    class Listener
    {
    public:
        /**
         Destructor
         */
        virtual ~Listener() {}
        
        /**
         Called when a new file is loaded so that the filename can be sent up to the GUI
         */
        virtual void audioFileNameChanged(const int audioFileIndex, const String &fileName)=0;
        
        /**
         Signals to listeners when the play state changes
         */
        virtual void audioFilePlayStateChanged(const int audioFileIndex, const PlayState state)=0;
    };
    
    /**
	 Adds a listener to recieve glove data
	 */
	void addListener(Listener *listenerToAdd);
	
    /**
	 Removes a listener
	 */
	void removeListener(Listener *listenerToRemove);
    
    /**
     Loads the specified file in the specified player
     */
    void loadAudioFile (const int playerIndex, const String& filePath);
    
    /**
     set the specified player playing at the specified gain
     */
    void play(const int playerIndex, const float gain = 1.f);
    
    /**
     Stops the specified player playing
     */
    void stop(const int playerIndex);
    
    /**
     toggle current play state
     */
    void togglePlayState(const int playerIndex);
    
    /**
     Stops all players
     */
    void stopAll();
    
    //AudioSouce Callbacks
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    virtual void releaseResources();
    virtual void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    //FilePlayer callback
    virtual void audioFileNameChanged(const AudioFilePlayer *player, const String &path);
    virtual void audioFilePlayStateChanged(const AudioFilePlayer *player, const AudioFilePlayer::PlayState state);
private:
    OwnedArray<AudioFilePlayer> players;
    
    MixerAudioSource mixerSource;   //mixes the players together
    
    //Listener
    ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePlayerManager)
};  


#endif //H_AUDIOFILEPLAYERMANAGER