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

#include <juce.h>

class PlayButton : public TextButton 
{
public:
    PlayButton(const String buttonName, const String toolTip) : TextButton(buttonName, toolTip) {}
    const Font 	getFont (){return Font ("Wingdings 3", jmin(getHeight(),getWidth())/1.5f, Font::plain);}
private:
};


/**
 Holds the gui components of a AudioFile selector
 */
class AudioFileSelector  :  public Component,
                            public FilenameComponentListener,
                            public Button::Listener
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
        virtual void audioFileNameChanged(const AudioFileSelector *selector, const String &fileName)=0;
        virtual void audioFilePlayButtonClicked(const AudioFileSelector *selector)=0;
    };
    
    /**
     Sets playstate -1 = disable 0 = stop 1 = play
     */
    void setPlayState(const int playState);
    
    /**
     Sets fileName in the box
     */
    void setFileName(const String& fileName);
    
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
    //==========================================================================
private:
	ScopedPointer <FilenameComponent> fileChooser;
	ScopedPointer <PlayButton> playButton; 
    
    //Listener
    ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileSelector)
};


#endif //H_AUDIOFILESELECTOR