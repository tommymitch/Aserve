/*
 *  AserveGui.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef H_ASERVEGUI
#define H_ASERVEGUI

#include <juce.h>
#include "Scope.h"
#include "AudioFileSelectorManager.h"
#include "BitwiseSelectorManager.h"

namespace GuiSettings
{
	static const Identifier SectionName = "GuiSettngs";
    
    enum Settings 
    {
        AudioFilesVisible,
        BitRepresentationVisible,
        
        numSettings
    };
    //surpress unused warnings
    static const Identifier Names[] = 
    {
        "AudioFilesVisible",
        "BitRepresentationVisible"
    };
    
    static const var Values[] = 
    {
        false,		//AudioFilesVisible
        false		//BitRepresentationVisible
    };
    
    static void buildGuiSettings(ValueTree* treeToFill)
	{
		ValueTree guiSettingsTree(GuiSettings::SectionName);
        treeToFill->addChild(guiSettingsTree, -1, 0);
        
		for (int p = 0; p < GuiSettings::numSettings; p++)
		{
			guiSettingsTree.setProperty(GuiSettings::Names[p], GuiSettings::Values[p], 0);
		}
        
        AudioFileSelectorManagerSettings::buildDefaultAudioFileSelectorManagerSettings(treeToFill);
        BitwiseSelectorManagerSettings::buildDefaultBitwiseSelectorManagerSettings(treeToFill);
	}
    
    static void initGuiSettings(ValueTree* treeToFill)
    {
        ValueTree guiSettingsTree = treeToFill->getChildWithName(GuiSettings::SectionName);
        AudioFileSelectorManagerSettings::initAudioFileSelectorManagerSettings(treeToFill);
        BitwiseSelectorManagerSettings::initBitwiseSelectorManagerSettings(treeToFill);
    }
}


/**
 Top level of all gui objects
 */
class AserveGui  :  public Component,
                    public ValueTree::Listener,
                    public AudioFileSelectorManager::Listener,
                    public BitwiseSelectorManager::Listener
{
public:
    /**
     Gui constructor
     */
    AserveGui();
    /**
     Gui destructor
     */
    ~AserveGui();
    
    /**
     Allows the gui to add all value tree listeners
     */
    void addValueTreeListeners(ValueTree* valueTree);
    
    /**
     Displays the audio settings window
     */
    void showAudioSettings(Component *centerComp, AudioDeviceManager* deviceManager);
    
    /**
     Returns pointer to the scope object
     */
    Scope* getScopePointer(){ return scope; }
    
    //callbacks==========================================================================
    void resized();
    //ValueTree==========================================================================
    virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    virtual void valueTreeChildAdded (ValueTree& parentTree,ValueTree& childWhichHasBeenAdded){}
    virtual void valueTreeChildRemoved (ValueTree& parentTree,ValueTree& childWhichHasBeenRemoved){}
    virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
    virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
    //AudioFileSelectorCallbacks=========================================================
    virtual void audioFileNameChanged(const int audioFileIndex, const String &fileName);
    virtual void audioFilePlayButtonClicked(const int audioFileIndex);
    //BitwiseSelector callbacks
    virtual void bitwiseSelectorClicked(const int x, const int y);
    
private:
    bool samplesVisible;
    bool bitwiseVisible;

    ScopedPointer<Scope> scope;
    ScopedPointer<AudioFileSelectorManager> audioFileSelectors;
    ScopedPointer<BitwiseSelectorManager> bitwiseSelectors;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveGui)
};


#endif //H_ASERVEAPP