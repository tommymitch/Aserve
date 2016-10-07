/*
 *  BitwiseSelectorManager.h
 *  Aserve
 *
 *  Created by tjmitche on 15/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_BITWISESELECTORMANAGER
#define H_BITWISESELECTORMANAGER

#include "../../JuceLibraryCode/JuceHeader.h"
#include "BitwiseSelector.h"


/**
 Top level of all gui objects
 */
class BitwiseSelectorManager :  public Component,
                                public ValueTree::Listener,
                                public BitwiseSelector::Listener
{
public:
    enum Constants
    {
        NumSelectors = 8
    };
    /**
     BitwiseSelectorManager constructor
     */
    BitwiseSelectorManager();
    /**
     BitwiseSelectorManager destructor
     */
    ~BitwiseSelectorManager();
    
    //Listener
    class Listener
    {
    public:
        /**
         Destructor
         */
        virtual ~Listener() {}
    
        /**
         Called when a button on the selector is clicked 
         bitwise index is the row and bitnumber is the bit that was clicked on that row
         */
        virtual void bitwiseSelectorClicked(const int x, const int y)=0;
    };
    
    /**
     Adds a listener to recieve glove data
     */
	void addListener(Listener *listenerToAdd);
	
    /**
	 Removes a listener
	 */
	void removeListener(Listener *listenerToRemove);
    
    //Component Callbacks
    void resized();
    //Valuetree callback==========================================================================
    virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    virtual void valueTreeChildAdded (ValueTree& parentTree,ValueTree& childWhichHasBeenAdded){}
    virtual void valueTreeChildRemoved (ValueTree& parentTree,ValueTree& childWhichHasBeenRemoved){}
    virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
    virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
    //Bitwise callback
    virtual void bitwiseSelectorClicked(const BitwiseSelector* selector, const int bitNumber);

    //==========================================================================
    
private:    
    
    OwnedArray<BitwiseSelector> selectors;
    
    //Listener
    ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitwiseSelectorManager)
};

namespace BitwiseSelectorManagerSettings
{
	static const Identifier SectionName = "BitwiseSelectorManagerSettngs";
    
    static const Identifier Names[] = 
    {
        "bitwise",
    };
    
    static const var Values[] = 
    {
        0    //Bitwise
    };
    
    static void buildDefaultBitwiseSelectorManagerSettings(ValueTree* treeToFill)
	{
		ValueTree bitwiseSelectorManagerSettingsTree(SectionName);
        treeToFill->addChild(bitwiseSelectorManagerSettingsTree, -1, 0);

        for (int y = 0; y < BitwiseSelectorManager::NumSelectors; ++y) 
        {
            bitwiseSelectorManagerSettingsTree.setProperty(Names[0].toString() + String(y), Values[0], 0);
        }
    }
    
    static void initBitwiseSelectorManagerSettings(ValueTree* treeToFill)
    {
        ValueTree bitwiseSelectorManagerSettingsTree = treeToFill->getChildWithName(BitwiseSelectorManagerSettings::SectionName);
        
        for (int y = 0; y < BitwiseSelectorManager::NumSelectors; ++y) 
        {
            bitwiseSelectorManagerSettingsTree.setProperty(Names[0].toString() + String(y), Values[0], 0);
        }
    }
}

#endif //BITWISESELECTORMANAGER