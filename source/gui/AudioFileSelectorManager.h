/*
 *  AudioFileSelectorManager.h
 *  Aserve
 *
 *  Created by tjmitche on 12/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOFILESELECTORMANAGER
#define H_AUDIOFILESELECTORMANAGER

#include <juce.h>
#include "AudioFileSelector.h"
#include "AudioFilePlayerManager.h"     //to access AudioFilePlayerManager::NumPlayers

/**
 Value Tree for the audio file player settings config.
 This is repeated plus an index for each row
 */

namespace AudioFileSelectorManagerSettings
{
	static const Identifier SectionName = "AudioFileSelectorManagerSettngs";
    
    
    enum Settings 
    {
        PlayState,
        FileName,
        
        numSettings
    };
    
    static const Identifier Names[] = 
    {
        "PlayState",
        "FileName"
    };
    
    static const var Values[] = 
    {
        AudioFilePlayerManager::Disable,    //PlayState
        ""                                  //FileName
    };
    
    static void buildDefaultAudioFileSelectorManagerSettings(ValueTree* treeToFill)
	{
		ValueTree audioFileSelectorManagerSettingsTree(SectionName);
        treeToFill->addChild(audioFileSelectorManagerSettingsTree, -1, 0);
        
        for (int i = 0; i < AudioFilePlayerManager::NumPlayers; ++i)
        {
            for (int p = 0; p < numSettings; p++)
            {
                audioFileSelectorManagerSettingsTree.setProperty(Names[p].toString()+String(i), Values[p], 0);
            }
        }
	}
}


/**
 Holds and manages an array of AudioFileSelectors
 */
class AudioFileSelectorManager  :   public Component,
                                    public ValueTree::Listener,
                                    public AudioFileSelector::Listener
{
public:
    
    /**
     constructor
     */
    AudioFileSelectorManager();
    /**
     destructor
     */
    ~AudioFileSelectorManager();
    
    class Listener
    {
    public:
        /**
         Destructor
         */
        virtual ~Listener() {}
        
        /**
         Called everytime a AudioFile is changed
         */
        virtual void audioFileNameChanged(const int audioFileIndex, const String &fileName)=0;
        /**
         Called when a button is clicked
         */
        virtual void audioFilePlayButtonClicked(const int audioFileIndex)=0;
    };
    
    /**
    Adds a listener to recieve glove data
    */
	void addListener(Listener *listenerToAdd);
	
    /**
	 Removes a listener
	 */
	void removeListener(Listener *listenerToRemove);
    
    //callbacks=================================================================
    void resized();
    //==========================================================================
    virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    virtual void valueTreeChildAdded (ValueTree& parentTree,ValueTree& childWhichHasBeenAdded){}
    virtual void valueTreeChildRemoved (ValueTree& parentTree,ValueTree& childWhichHasBeenRemoved){}
    virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
    virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
    //===========================================================================
    virtual void audioFileNameChanged(const AudioFileSelector *selector, const String &fileName);
    virtual void audioFilePlayButtonClicked(const AudioFileSelector *selector);
    
private:
    OwnedArray <AudioFileSelector> selectors;
    //Listener
    ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileSelectorManager)
};


#endif //H_AUDIOFILESELECTORMANAGER