/*
 *  AserveComponent.cpp
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#include "AserveComponent.h"

AserveComponent::AserveComponent()
{
    aserve = new Aserve();
    addAndMakeVisible (aserve);
}
AserveComponent::~AserveComponent()
{

}

//==============================================================================
void AserveComponent::resized()
{
    aserve->setBounds(getLocalBounds());
}

void AserveComponent::paint (Graphics &g)
{

}

//==============================================================================
const StringArray AserveComponent::getMenuBarNames()
{
	const char* const names[] = { "File", 0 };
	
	return StringArray (names);
}

const PopupMenu AserveComponent::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu;
	
	if (topLevelMenuIndex == 0)
	{
        menu.addItem(AUDIOPREFS, "Audio Prefrences", true, false);
		menu.addSeparator();
	}
	return menu;
}

void AserveComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
	if (topLevelMenuIndex == FILEMENU) 
    {
		switch (menuItemID) 
        {

            case AUDIOPREFS:
                //gloverComponent->showAudioPrefs();
				break;
			default:
				break;
		}
	}
}

