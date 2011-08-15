/*
 *  AudioFilePlayer.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOFILEPLAYER
#define H_AUDIOFILEPLAYER

#include <juce.h>

/**
 Top level of the model class where all internal data is stored
 */
class AudioFilePlayer : public AudioSource,
                        public ChangeListener
{
public:
    /**
     Gui constructor
     */
    AudioFilePlayer();
    /**
     Gui destructor
     */
    ~AudioFilePlayer();
    
    /**
     Enum used in the callback
     */
    enum PlayState
    {
        Disable = -1,
        Stop,
        Play,
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
        virtual void audioFileNameChanged(const AudioFilePlayer *player, const String &path)=0;
    
        /**
         Signals to listeners when the play state changes
         */
        virtual void audioFilePlayStateChanged(const AudioFilePlayer *player, const PlayState state)=0;
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
     Loads the specified file ready for playback
     */
    void loadAudioFile (const String& path);
    
    /**
     Sets the player playing at the optional specified gain
     */
    void play(const float gain = 1.f);
    
    /**
     Stops the player
     */
    void stop();
    
    /**
     Toggles the current play state
     */
    void togglePlayState();
        
    //AudioSouce Callbacks
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    virtual void releaseResources();
    virtual void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    //Changelistener Callback
    void changeListenerCallback (ChangeBroadcaster* source);
    
private:
    // this controls the playback of a positionable audio stream, handling the
	// starting/stopping and sample-rate conversion
	AudioTransportSource transportSource;		
	// this is the actual stream that's going to read from the audio file.
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	File currentFile;
    
    //Listener
    ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePlayer)
};  


#endif //H_AUDIOFILEPLAYER