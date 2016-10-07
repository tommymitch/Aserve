/*
 *  AudioFileSelector.h
 *  Aserve
 *
 *  Created by tjmitche on 12/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOFILESELECTOR
#define H_AUDIOFILESELECTOR

#include "../../JuceLibraryCode/JuceHeader.h"

class PlayButton : public TextButton 
{
public:
    PlayButton(const String buttonName, const String toolTip) : TextButton(buttonName, toolTip) {}
    Font 	getFont (){return Font ("Wingdings 3", jmin(getHeight(),getWidth())/1.5f, Font::plain);}
private:
};

/**
 Holds the gui components of a AudioFile selector
 */
class AudioFileSelector  :  public Component,
                            public FilenameComponentListener,
                            public Button::Listener,
                            public MessageListener
{
public:
    /**
    constructor
     */
    AudioFileSelector();
    /**
     destructor
     */
    ~AudioFileSelector();
    
    /**
     Sets playstate -1 = disable 0 = stop 1 = play
     */
    void setPlayState(const int playState);
    
    /**
     Sets fileName in the box
     */
    void setFileName(const String& fileName);
    
    //Listener=====================================================================
    class Listener
    {
    public:
        /**
         Destructor
         */
        virtual ~Listener() {}
        
        /**
         one of the buttons is pressed or a file is loaded
         */
        virtual void audioFileNameChanged(const AudioFileSelector* selector, const String &fileName)=0;
        virtual void audioFilePlayButtonClicked(const AudioFileSelector* selector)=0;
    };
    
    /**
	 Adds a listener to recieve glove data
	 */
	void addListener(Listener *listenerToAdd);
	
    /**
	 Removes a listener
	 */
	void removeListener(Listener *listenerToRemove);
    
    //callbacks==========================================================================
    virtual void resized();
    virtual void buttonClicked (Button* button);
    virtual void filenameComponentChanged (FilenameComponent*);
    //first int of the message type signals the message
    enum MessageTypes 
    {
        PlayMessage,
        FileMessage,
        
        NumTypes
    };
    virtual void handleMessage (const Message& message);
    //==========================================================================
private:
	ScopedPointer <FilenameComponent> fileChooser;
	ScopedPointer <PlayButton> playButton; 
    
    //Listener
    ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileSelector)
};

class AudioFileMessage : public Message
{
public:
    AudioFileMessage(const AudioFileSelector::MessageTypes type_, const int playState_, const String path_ = String::empty) 
    : type(type_), playState(playState_), path(path_) {}
    const AudioFileSelector::MessageTypes type;
    const int playState;
    const String path;
};



#endif //H_AUDIOFILESELECTOR