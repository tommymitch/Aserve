/*
 *  AserveComponent.h
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#ifndef H_ASERVECOMPONENT
#define H_ASERVECOMPONENT

#include <juce.h>
#include "Aserve.h"

class AserveComponent  : public Component,
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
		
		NUMFILEMENUITEMS
	};
	//==============================================================================
	AserveComponent();
	~AserveComponent();
	//==============================================================================
	void resized();
	void paint (Graphics &g); 
    //==============================================================================
    const StringArray getMenuBarNames();
    const PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
	void menuItemSelected (int menuItemID, int topLevelMenuIndex);
	//==============================================================================
private:
	//==============================================================================
	//Add data members here
    ScopedPointer<Aserve> aserve;
    TooltipWindow tooltipWindow;
};




#endif // H_ASERVECOMPONENT