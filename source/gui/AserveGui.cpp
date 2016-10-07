/*
 *  AserveGui.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveGui.h"
#include "../controller/AserveController.h"

AserveGui::AserveGui() :  samplesVisible(false), bitwiseVisible(false)
{
    scope = new Scope();
    addAndMakeVisible(scope);
    
    errorBox = new ErrorTextBox();
    addAndMakeVisible(errorBox);
    
    connectionIndicator = new Label("GloveIndicator","disconnected");
	connectionIndicator->setJustificationType(Justification::centred);
	connectionIndicator->setColour(Label::backgroundColourId, Colours::red);
	addAndMakeVisible(connectionIndicator);
    
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

void AserveGui::showErrorMessage(const String& message)
{
    errorBox->addErrorMessage(message);
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
	
	
	DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComp, Colours::azure, true);
}

//callbacks
void AserveGui::resized()
{
    int w = getWidth();
    int h = getHeight();
    
    connectionIndicator->setBounds(1,h-20,w-2,19);
    h-=20;
    
    if (samplesVisible && bitwiseVisible) 
    {
        int x = w;
        int y = h/3;
        scope->setBounds(0,0,x,y);
        errorBox->setBounds(0,0,x,y);
        audioFileSelectors->setBounds(2,y+2,x-4,y-4);
        bitwiseSelectors->setBounds(2,2*y+2,x-4,y-4);
    }
    else if(samplesVisible)
    {
        int x = w;
        int y = h/2;
        scope->setBounds(0,0,x,y);
        errorBox->setBounds(0,0,x,y);
        audioFileSelectors->setBounds(2,y+2,x-4,y-4);
    }
    else if(bitwiseVisible)
    {
        int x = w;
        int y = h/2;
        scope->setBounds(0,0,x,y);
        errorBox->setBounds(0,0,x,y);
        bitwiseSelectors->setBounds(2,y+2,x-4,y-4);
    }
    else 
    {
        scope->setBounds(0,0,w,h);
        errorBox->setBounds(0,0,w,h+20);
    }
}

void AserveGui::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged.getType().toString() == GuiSettings::SectionName) 
    {
        if (property == GuiSettings::Names[GuiSettings::AudioFilesVisible]) 
        {
            samplesVisible = treeWhosePropertyHasChanged.getProperty(property);
            audioFileSelectors->setVisible(samplesVisible);
            resized();
        }
        else if (property == GuiSettings::Names[GuiSettings::BitRepresentationVisible]) 
        {
            bitwiseVisible = treeWhosePropertyHasChanged.getProperty(property);
            bitwiseSelectors->setVisible(bitwiseVisible);
            resized();
        }
        else if(property == GuiSettings::Names[GuiSettings::NetworkConnectionIndicator])
        {
            bool connection = treeWhosePropertyHasChanged.getProperty(property);
            postMessage(new GuiMessage(connection));
        }
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

void AserveGui::handleMessage (const Message& message)
{
    const GuiMessage& guiMessage = dynamic_cast<const GuiMessage &> (message);
    connectionIndicator->setText(guiMessage.connected ? "connected" : "disconnected", false);
    connectionIndicator->setColour(Label::backgroundColourId, guiMessage.connected ? Colours::green : Colours::red);
}