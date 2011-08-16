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
    //Create all objects
    model = new AserveModel();
    
    gui = new AserveGui();
    addAndMakeVisible (gui);
    
    network = new AserveNetwork();
    
    audio = new AserveAudio();
    
    //create and set up the controller singleton
    AserveController *controller = AserveController::getInstance();
    if (controller) 
    {
        controller->setModelObject(model);
        controller->setGuiObject(gui);
        controller->setNetworkObject(network);
        controller->setAudioObject(audio);
    }
    
    
    //add the scope to the audio object
    audio->setScopeObject(gui->getScopePointer());
    
    //set the valuetree listeners
    //model->getValueTree()->addListener(gui);
    gui->addValueTreeListeners(model->getValueTree());
    
    //populate the value tree
    GuiSettings::buildGuiSettings(model->getValueTree());
}

AserveComponent::~AserveComponent()
{
    audio->setScopeObject(NULL);
    
    AserveController *controller = AserveController::getInstance();
    if (controller) 
    {
        controller->setAudioObject(NULL);
        controller->setNetworkObject(NULL);
        controller->setModelObject(NULL);        
    }
    
    //It would be good to delete the network object
    network = NULL; //stop the network activity as this could still be buzzing away could be trying to access the controller after the controllers been deleted - this could be the case for the other objects too.

    AserveController::deleteInstance();
}

//==============================================================================
void AserveComponent::resized()
{
    gui->setBounds(getLocalBounds());
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

        AserveController *ac = AserveController::getInstance();
        if (ac) 
        {
            bool state = ac->getModelParameter(GuiSettings::SectionName,GuiSettings::Names[GuiSettings::AudioFilesVisible]);
            menu.addItem(AUDIOFILES, "Show Audio Files", true, state);
            state = ac->getModelParameter(GuiSettings::SectionName,GuiSettings::Names[GuiSettings::BitRepresentationVisible]);
            menu.addItem(BITREPRESENTATION, "Show Bit Representation", true, state);
        }
	}
	return menu;
}

void AserveComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
	if (topLevelMenuIndex == FILEMENU) 
    {
        if (menuItemID == AUDIOPREFS) 
        {
            gui->showAudioSettings(this, audio->getDeviceManager());
        }
        else if (menuItemID == AUDIOFILES) 
        {
            AserveController *ac = AserveController::getInstance();
            if (ac) 
            {
                bool newState = !ac->getModelParameter (GuiSettings::SectionName, GuiSettings::Names[GuiSettings::AudioFilesVisible]);
                ac->setModelParameter (GuiSettings::SectionName, GuiSettings::Names[GuiSettings::AudioFilesVisible], newState);
            }
            
        }
        else if (menuItemID == BITREPRESENTATION) 
        {
            AserveController *ac = AserveController::getInstance();
            if (ac) 
            {
                bool newState = !ac->getModelParameter (GuiSettings::SectionName, GuiSettings::Names[GuiSettings::BitRepresentationVisible]);
                ac->setModelParameter (GuiSettings::SectionName, GuiSettings::Names[GuiSettings::BitRepresentationVisible], newState);
            }
        }
	}
}
