/*
 *  AserveComponent.h
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#ifndef H_ASERVECOMPONENT
#define H_ASERVECOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "model/AserveModel.h"
#include "controller/AserveController.h"
#include "gui/AserveGui.h"
#include "network/AserveNetwork.h"
#include "audio/AserveAudio.h"

//#include "Aserve.h"

class AserveComponent  :    public Component,
                            public MenuBarModel
{	
public:
    enum Menus
	{
		FILEMENU=0,
		
		NUMMENUS
	};
    
    enum FileMenuItems 
	{
        AUDIOPREFS = 1,
        AUDIOFILES,
        BITREPRESENTATION,
		
		NUMFILEMENUITEMS
	};
	//==============================================================================
	AserveComponent();
	~AserveComponent();
	//==============================================================================
	void resized();
	void paint (Graphics &g); 
    //==============================================================================
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
	void menuItemSelected (int menuItemID, int topLevelMenuIndex);
	//==============================================================================
private:
	//==============================================================================
	//Add data members here
    ScopedPointer<AserveModel> model;             //where all data is stored
    ScopedPointer<AserveGui> gui;                 //all gui objects
    ScopedPointer<AserveNetwork> network;         //where all network activity is managed
    ScopedPointer<AserveAudio> audio;             //where all midi and audio is done
    
    TooltipWindow tooltipWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveComponent)
};




#endif // H_ASERVECOMPONENT