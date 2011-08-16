/*
 *  AserveGui.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveGui.h"
#include "AserveController.h"

AserveGui::AserveGui() :  samplesVisible(false), bitwiseVisible(false)
{
    scope = new Scope();
    addAndMakeVisible(scope);
    
    audioFileSelectors = new AudioFileSelectorManager();
    audioFileSelectors->addListener(this);
    addAndMakeVisible(audioFileSelectors);
    
    bitwiseSelectors = new BitwiseSelectorManager();
    bitwiseSelectors->addListener(this);
    addAndMakeVisible(bitwiseSelectors);
    
}

AserveGui::~AserveGui()
{
    audioFileSelectors->removeListener(this);
}

void AserveGui::addValueTreeListeners(ValueTree* valueTree)
{
    valueTree->addListener(this);
    valueTree->addListener(audioFileSelectors);
    valueTree->addListener(bitwiseSelectors);
}

void AserveGui::showAudioSettings(Component *centerComp, AudioDeviceManager* deviceManager)
{
    AudioDeviceSelectorComponent audioSettingsComp (*deviceManager,
													0, 0,
													2, 2,
													true, true,
													true, true);
	
	audioSettingsComp.setSize (500, 250);
	
	
	DialogWindow::showModalDialog (T("Audio Settings"), &audioSettingsComp, centerComp, Colours::azure, true);
}

//callbacks
void AserveGui::resized()
{
    if (samplesVisible && bitwiseVisible) 
    {
        int x = getWidth();
        int y = getHeight()/3;
        scope->setBounds(0,0,x,y);
        audioFileSelectors->setBounds(2,y+2,x-4,y-4);
        bitwiseSelectors->setBounds(2,2*y+2,x-4,y-4);
    }
    else if(samplesVisible)
    {
        int x = getWidth();
        int y = getHeight()/2;
        scope->setBounds(0,0,x,y);
        audioFileSelectors->setBounds(2,y+2,x-4,y-4);
    }
    else if(bitwiseVisible)
    {
        int x = getWidth();
        int y = getHeight()/2;
        scope->setBounds(0,0,x,y);
        bitwiseSelectors->setBounds(2,y+2,x-4,y-4);
    }
    else 
    {
        scope->setBounds(getLocalBounds());
    }
}

void AserveGui::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.getType().toString() == GuiSettings::SectionName) 
    {
        if (property == GuiSettings::Names[GuiSettings::AudioFilesVisible]) 
        {
            samplesVisible = treeWhosePropertyHasChanged.getProperty(GuiSettings::Names[GuiSettings::AudioFilesVisible]);
            audioFileSelectors->setVisible(samplesVisible);
        }
        else if (property == GuiSettings::Names[GuiSettings::BitRepresentationVisible]) 
        {
            bitwiseVisible = treeWhosePropertyHasChanged.getProperty(GuiSettings::Names[GuiSettings::BitRepresentationVisible]);
            bitwiseSelectors->setVisible(bitwiseVisible);
        }
        resized();
    }    
}

void AserveGui::audioFileNameChanged(const int audioFileIndex, const String &path)
{
    AserveController* ac = AserveController::getInstance();
    if (ac) 
        ac->loadFile(AserveController::Gui, audioFileIndex, path);
}

void AserveGui::audioFilePlayButtonClicked(const int audioFileIndex)
{
    AserveController* ac = AserveController::getInstance();
    if (ac) 
        ac->playFile(AserveController::Gui, audioFileIndex);
}

void AserveGui::bitwiseSelectorClicked(const int x, const int y)
{
    AserveController* ac = AserveController::getInstance();
    if (ac) 
        ac->bitwise(AserveController::Gui, x, y);
}