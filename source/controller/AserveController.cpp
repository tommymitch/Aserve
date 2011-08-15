/*
 *  AserveController.cpp
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011. All rights reserved.
 *
 */

#include "AserveController.h"

juce_ImplementSingleton (AserveController)

AserveController::AserveController() : model(NULL), audio(NULL), network(NULL)
{
    
}

AserveController::~AserveController()
{
    
    clearSingletonInstance();
}

//Model
const var& AserveController::getModelParameter(const Identifier& group, const Identifier& parameter) const
{
    ValueTree vt = model->getValueTree()->getChildWithName(group);
    return vt.getProperty(parameter);
}

void AserveController::setModelParameter(const Identifier& group, const Identifier& parameter, const var& newValue)
{
    ValueTree vt = model->getValueTree()->getChildWithName(group);
    vt.setProperty (parameter, newValue,0);
}

void AserveController::setAudioFileParameter(const int fileIndex, const String& path)
{
    ValueTree vt = model->getValueTree()->getChildWithName(AudioFileSelectorManagerSettings::SectionName);
    vt.setProperty (AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::FileName].toString()+String(fileIndex), path,0);
}

void AserveController::setAudioFileParameter(const int fileIndex, const int playState)
{
    ValueTree vt = model->getValueTree()->getChildWithName(AudioFileSelectorManagerSettings::SectionName);
    vt.setProperty (AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::PlayState].toString()+String(fileIndex), playState,0);
}

//Network
void AserveController::sendNetworkMessage(const String& message)
{
    if (network)
        network->sendMessage(message);
}

void AserveController::parseNetworkMessage(const String& message)
{
    if(message.startsWith("sub"))
	{
		//owner.sendGuiActionMessage(string);
	}
	else
	{
        if (audio) 
            audio->parseMessage(message);
	}
}

//Audio
void AserveController::stopAllAudio()
{
    if (audio)
        audio->stopAll();
}

void AserveController::audioFile(const Sources source, const int fileIndex)
{
    if (source == Gui) 
    {
        if (audio)
            audio->audioFile(fileIndex);
    }
    
}

void AserveController::audioFile(const Sources source, const int fileIndex, const String &newFilePath)
{
    if (source == Gui) 
    {
        setAudioFileParameter(fileIndex, newFilePath);  //need to keep the tree in sync with the GUI - has to happen first is it may be reset by the audio object
        if (audio)
            audio->audioFile(fileIndex, newFilePath);
        
    }
    
}
