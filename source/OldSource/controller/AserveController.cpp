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

AserveController::AserveController() : model(NULL), gui(NULL), audio(NULL), network(NULL)
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
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(group);
        vt.setProperty (parameter, newValue,0);
    }
    
}

void AserveController::setAudioFileParameter(const int fileIndex, const String& path)
{
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(AudioFileSelectorManagerSettings::SectionName);
        vt.setProperty (AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::FileName].toString()+String(fileIndex), path,0);
    }
}

void AserveController::setAudioFileParameter(const int fileIndex, const int playState)
{
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(AudioFileSelectorManagerSettings::SectionName);
        vt.setProperty (AudioFileSelectorManagerSettings::Names[AudioFileSelectorManagerSettings::PlayState].toString()+String(fileIndex), playState,0);
    }
}

void AserveController::toggleBitwiseParameter(const int x, const int y)
{
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(BitwiseSelectorManagerSettings::SectionName);
        String propertyname(BitwiseSelectorManagerSettings::Names[0].toString()+String(y));
        BigInteger modelValue((int32)vt.getProperty (propertyname));
        modelValue.setBit(x,!modelValue[x]);//toggle
        vt.setProperty (propertyname, modelValue.toInteger(), 0);
        String message(String("sequence") + ":" + String(y) + ":" + String((int32)vt.getProperty (propertyname)));
        sendNetworkMessage(message);
    }
   
    
}

void AserveController::setBitwiseParameter(const int bitwiseSelector, const int newValue)
{
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(BitwiseSelectorManagerSettings::SectionName);
        String propertyname(BitwiseSelectorManagerSettings::Names[0].toString()+String(bitwiseSelector));
        vt.setProperty (propertyname, newValue, 0);
        String message(String("sequence") + ":" + String(bitwiseSelector) + ":" + String((int32)vt.getProperty (propertyname)));
        sendNetworkMessage(message);
    }
}

void AserveController::setNetworkConnectionParameter(bool isConnected)
{
    if (model) 
    {
        ValueTree vt = model->getValueTree()->getChildWithName(GuiSettings::SectionName);
        vt.setProperty (GuiSettings::Names[GuiSettings::NetworkConnectionIndicator].toString(), isConnected,0);
    }
}

int AserveController::getBitwiseParameter(const int bitwiseSelector)
{
    ValueTree vt = model->getValueTree()->getChildWithName(BitwiseSelectorManagerSettings::SectionName);
    String propertyname(BitwiseSelectorManagerSettings::Names[0].toString()+String(bitwiseSelector));
    return vt.getProperty (propertyname);
}

//Network
void AserveController::sendNetworkMessage(const String& message)
{
    if (network)
        network->sendMessage(message);
}

bool AserveController::checkRange(const String &parameter, const float value, const float min, const float max)
{
    //IMPORTANT - NEED TO FIND A CROSS PLATFORM METHOD FOR CHECKING FOR INF and NaN! as 
	//these may get through this check.
	bool inRange = (value > max || value < min) ? false : true;
    
    if (!inRange) 
    {
        if (gui) 
        {
            gui->showErrorMessage(parameter + " Out of Range: " + String(value) + "  " + "Range: " + String(min) + "-" + String(max) + "\n");
        }
    }
	
	return inRange;
    
}

void AserveController::parseNetworkMessage(const String& message)
{
    if(message.startsWithChar('a'))
	{
		parseAudioNetworkMessage(message.fromFirstOccurrenceOf(":", false, false));
	}
	else if(message.startsWithChar('g'))
	{
        parseGuiNetworkMessage(message.fromFirstOccurrenceOf(":", false, false));
	}
    else 
    {
        std::cout << "OtherNetworkMessages!!!:" << message << "\n";
    }

}

void AserveController::parseAudioNetworkMessage(const String& message)
{
    if(message.startsWithChar('o'))//oscillator
    {
        String oscillatorString(message.fromFirstOccurrenceOf(":", false, false)); 
        int index = oscillatorString.getIntValue();
        oscillatorString = oscillatorString.fromFirstOccurrenceOf(":", false, false); 
        float frequency = oscillatorString.getFloatValue();
        oscillatorString = oscillatorString.fromFirstOccurrenceOf(":", false, false);
        float amplitude = oscillatorString.getFloatValue();
        oscillatorString = oscillatorString.fromFirstOccurrenceOf(":", false, false);
        int waveform = oscillatorString.getIntValue();
        
        if(   checkRange("Oscillator Number", index, 0, OscillatorManager::NumOscillators-1) 
           && checkRange("Oscillator Frequency", frequency, 0.f, 20000.f) 
           && checkRange("Oscillator Amplitude", amplitude, 0.f, 1.f) 
           && checkRange("Oscillator Waveform", waveform, 0, OscillatorManager::NumWaveforms-1)  )
        {
            if (audio) 
                audio->setOscillator(index, frequency, amplitude, waveform);
            
        }
    }
    else if(message.startsWithChar('p'))//play sample
    {
        String playString(message.fromFirstOccurrenceOf(":", false, false)); 
        int index = playString.getIntValue();
        playString = playString.fromFirstOccurrenceOf(":", false, false); 
        float gain = playString.getFloatValue();
        
        if(   checkRange("Sample Number", index, 0, AudioFilePlayerManager::NumPlayers-1) 
           && checkRange("Sample Gain", gain, 0.f, 1.f) )
        {
            playFile(Network, index, gain);
        }
    }
    else if(message.startsWithChar('l'))//load sample
    {
        String loadString(message.fromFirstOccurrenceOf(":", false, false)); 
        int index = loadString.getIntValue();
        loadString = loadString.fromFirstOccurrenceOf(":", false, false); 
        
        if(checkRange("Sample Number", index, 0, AudioFilePlayerManager::NumPlayers-1))
        {
            loadFile(Network, index, loadString);
        }
    }
    else if(message.startsWithChar('m'))//load sample
    {
        String midiString(message.fromFirstOccurrenceOf(":", false, false)); 
        unsigned char status = midiString.getIntValue();
        midiString = midiString.fromFirstOccurrenceOf(":", false, false); 
        unsigned char data1 = midiString.getIntValue();
        midiString = midiString.fromFirstOccurrenceOf(":", false, false);
        unsigned char data2 = midiString.getIntValue();
        
        if(   checkRange("MIDI Status Byte", status, 128, 255) 
           && checkRange("MIDI Data Byte 1", data1, 0, 127) 
           && checkRange("MIDI Data Byte 2", data2, 0, 127) )
        {
            if (audio) 
                audio->setMidi(status, data1, data2);
            
        }
    }

}

void AserveController::parseGuiNetworkMessage(const String& message)
{
    if(message.startsWithChar('b'))
    {
        String bitwiseString(message.fromFirstOccurrenceOf(":", false, false)); 
        int index = bitwiseString.getIntValue();
        bitwiseString = bitwiseString.fromFirstOccurrenceOf(":", false, false); 
        int newValue = bitwiseString.getIntValue();
        
        if(checkRange("Row number", index, 0, BitwiseSelectorManager::NumSelectors) 
           && checkRange("Sequence value", newValue, 0, 65535))
        {
            setBitwiseParameter(index, newValue);            
        }
    }
}

//Audio
void AserveController::reset()
{
    if (audio)
        audio->stopAll();
    
    if (gui && model)
        GuiSettings::initGuiSettings(model->getValueTree());
    
    setNetworkConnectionParameter(false);
        
}

void AserveController::playFile(const Sources source, const int fileIndex)
{
    if (source == Gui) 
    {
        if (audio)
            audio->playFile(fileIndex);
    }
    
}

void AserveController::playFile(const Sources source, const int fileIndex, const float gain)
{
    if (source == Network) 
    {
        if (audio)
            audio->playFile(fileIndex, gain);
    }
}

void AserveController::loadFile(const Sources source, const int fileIndex, const String &newFilePath)
{
    if (source == Gui) 
    {
        setAudioFileParameter(fileIndex, newFilePath);  //need to keep the tree in sync with the GUI - has to happen first is it may be reset by the audio object
    }
    if (audio)
        audio->loadFile(fileIndex, newFilePath);
        
    
    
}
//I don't think the source matters here?
void AserveController::bitwise(const Sources source, const int x, const int y)
{
    if (source == Gui) 
    {
        toggleBitwiseParameter(x, y);
    }
}

int AserveController::getBitwise(const int selectorIndex)
{
    return getBitwiseParameter(selectorIndex);
}
