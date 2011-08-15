/*
 *  AserveAudio.cpp
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveAudio.h"
#include "AserveController.h"

AserveAudio::AserveAudio() : scopeObject(NULL)
{
    audioFiles = new AudioFilePlayerManager();
    audioFiles->addListener(this);
    oscillators = new OscillatorManager();
    
    const String error = audioDeviceManager.initialise (1, /* number of input channels */
                                                        2, /* number of output channels */
                                                        0, /* no XML settings.. */
                                                        true  /* select default device on failure */);
	
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Aserve"),
									 T("Couldn't open an output device!\n\n") + error);
	}
	else
	{
		// add the audio sources to our mixer..
        mixerSource.addInputSource (audioFiles, false);
		mixerSource.addInputSource (oscillators, false);
		
		// ..and connect the mixer to our source player.
		audioSourcePlayer.setSource (&mixerSource);
		audioSourcePlayer.setGain(1.0f);
		
		// start the IO device pulling its data from our callback..
		audioDeviceManager.addAudioCallback (this);
		
		// we also need to pick up the midi
		audioDeviceManager.addMidiInputCallback (String::empty, this);
		
//		StringArray s(MidiInput::getDevices());
//		audioDeviceManager.setMidiInputEnabled(s[0],true);
//		s = MidiOutput::getDevices();
//		audioDeviceManager.setDefaultMidiOutput(s[0]);
	}
    
}

AserveAudio::~AserveAudio()
{
    audioDeviceManager.removeMidiInputCallback (String::empty,this);
	audioDeviceManager.removeAudioCallback (this);
    audioSourcePlayer.setSource (0);
    mixerSource.removeAllInputs();
    audioFiles->removeListener(this);
}

void AserveAudio::parseMessage(const String& message)
{
    String string = message;
    
    if(string.startsWith(T("osc")))
    {
        //owner.appendMessage(string);
        String oscString = string.substring(string.indexOfChar(':')+1);//go from first number
        int oscillatorNumber = oscString.getIntValue();
        oscString = oscString.substring(oscString.indexOfChar(':')+1);// second
        float freq = oscString.getFloatValue();
        oscString = oscString.substring(oscString.indexOfChar(':')+1);// second
        float amp = oscString.getFloatValue();
        oscString = oscString.substring(oscString.indexOfChar(':')+1);// second
        int wave = oscString.getIntValue();
        
        //sendActionMessage(String(freq)+T(":")+String(amp));
        
        if(inRange("Osc Number", oscillatorNumber, 0, 15) && inRange("Osc Freq", freq, 0.f, 20000.f) 
           && inRange("Osc Amp", amp, 0.f, 1.f) && inRange("Osc Wave", wave, 0, 2))
        {
            if(amp > 0.f)
            {
                oscillators->setAmplitude(oscillatorNumber,amp);
                oscillators->setFrequency(oscillatorNumber,freq);
                oscillators->setWave(oscillatorNumber,wave);
            }
            else
            {
                oscillators->stop(oscillatorNumber);
            }
            
            oscillators->setFrequency(oscillatorNumber,freq);
            oscillators->setWave(oscillatorNumber,wave);
        }
        
    }
    else if(string.startsWith(T("midi")))
    {
        //owner.appendMessage(string);
        
        String oscString = string.substring(string.indexOfChar(':')+1);//go from first number
        const int byte1 = oscString.getIntValue();
        oscString = oscString.substring(oscString.indexOfChar(':')+1);//from the second num
        const int byte2 = oscString.getIntValue();
        oscString = oscString.substring(oscString.indexOfChar(':')+1);// third
        const int byte3 = oscString.getIntValue();
        
        MidiMessage midiMessage(byte1, byte2, byte3);
        (audioDeviceManager.getDefaultMidiOutput())->sendMessageNow(midiMessage);
    }
    else if(string.startsWith(T("samp")))
    {
        //owner.appendMessage(string);
        String sampString = string.substring(string.indexOfChar(':')+1);//go from first number
        const int sampleCommand = sampString.getIntValue();
        sampString = sampString.substring(sampString.indexOfChar(':')+1);// second
        if(sampleCommand == PLAYFILE)
        {
            const int sampleNumber = sampString.getIntValue();
            sampString = sampString.substring(sampString.indexOfChar(':')+1);// third
            const float gain = sampString.getFloatValue();
            if(inRange("Sample number", sampleNumber, 0, 7) && inRange("Sample gain", gain, 0.0, 1.0))
            {
                if(gain > 0.00001)
                {	
                    audioFiles->play(sampleNumber, gain);
                }
                else
                {
                    audioFiles->stop(sampleNumber);
                }
            }
        }
        else if(sampleCommand == LOADFILE)
        {
            const int sampleNumber = sampString.getIntValue();
            sampString = sampString.substring(sampString.indexOfChar(':')+1);// third should be the path
            if(inRange("Sample number", sampleNumber, 0, 7))
            {
                std::cout << "LoadingSample:" << sampString << "\n";
                audioFiles->loadAudioFile(sampleNumber, sampString);//line below triggers this
                                                                    //sendActionMessage(T("seq:load:")+String(sampleNumber)+T(":")+sampString);
            }
        }
        //both LOADFILE and PLAYFILE require the spamle window buttons updating
        /*owner.sampleSelectorWindow->updateButtons();*///post this to the gui object
                                                        //sendActionMessage(T("sub:updatebuttons"));//updateButtons();  //need to post this to gui!
                                                        //THE LOADAUDIOFILE MESSAGE SHOULD UPDATE THE BUTTONS THROUGH THE VALUE TREE
        
    }	
    else if(string.startsWith(T("stop")))
    {
        stopAll();
    }
    else if(string.startsWith(T("vol")))
    {
        String volString = string.substring(string.indexOfChar(':')+1);//go from first number
        double vol = volString.getDoubleValue();
        if(inRange("volume value", vol, 0.0, 1.0))
        {
            audioSourcePlayer.setGain((float)(vol*vol*vol));
        }
    }
}

bool AserveAudio::inRange(const char *parameter, double value, double min, double max)
{
    //IMPORTANT - NEED TO FIND A CROSS PLATFORM METHOD FOR CHECKING FOR INF and NaN! as 
	//these may get through this check.
	bool inrange = false;
	(value > max || value < min) ? inrange = false : inrange = true;
	
//	if(!inrange)        //NEED TO GET THIS ON THE GUI SOMEHOW
//		sendActionMessage(String(parameter) + T(" Out of Range: ") + String(value) + T("  ")
//                          + T("Range: ") + String(min) + T("-") + String(max));
	
	return inrange;
    
}

void AserveAudio::stopAll(void)
{
    oscillators->stopAll();
    audioFiles->stopAll();
}

void AserveAudio::audioFile(const int audioFileIndex)
{
    audioFiles->togglePlayState(audioFileIndex);
}

void AserveAudio::audioFile(const int audioFileIndex, bool shouldPlay)
{
    if (shouldPlay) 
        audioFiles->play(audioFileIndex, 1.f);
    else 
        audioFiles->stop(audioFileIndex);

}

void AserveAudio::audioFile(const int audioFileIndex, const String &newFilePath)
{
    audioFiles->loadAudioFile(audioFileIndex, newFilePath);
}


//Audio 
void AserveAudio::audioDeviceAboutToStart (AudioIODevice* device)
{
    if (scopeObject) 
        scopeObject->audioDeviceAboutToStart (device);
    audioSourcePlayer.audioDeviceAboutToStart (device);
    std::cout << "Audio Start\n";
}

void AserveAudio::audioDeviceStopped()
{
    if (scopeObject) 
        scopeObject->audioDeviceStopped ();
    audioSourcePlayer.audioDeviceStopped ();
    std::cout << "Audio Stop\n";
}

void AserveAudio::audioDeviceIOCallback (const float** inAudio,int numInChannels,float** outAudio,int numOutAudio,int numSamples)
{
    audioSourcePlayer.audioDeviceIOCallback (inAudio, numInChannels, outAudio, numOutAudio, numSamples);
    if (scopeObject) 
        scopeObject->audioDeviceIOCallback (inAudio, numInChannels, outAudio, numOutAudio, numSamples);
}

//Midi
void AserveAudio::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message)
{
    unsigned char *rawData;	
	rawData = message.getRawData();
	
	String srawData("midiraw");
	for(int i = 0; i < message.getRawDataSize();i++)
	{
		srawData = srawData + String(":") + String(*rawData);
		rawData++;
	}
	
	if(message.getRawDataSize() == 2)		//if only one databyte pad the other
		srawData = srawData + String(":") + String(-1);
    
    AserveController* ac = AserveController::getInstance();
    if (ac)
        ac->sendNetworkMessage(srawData);
}

//AudioFiles
void AserveAudio::audioFileNameChanged(const int audioFileIndex, const String &path)
{
    AserveController* ac = AserveController::getInstance();
    if (ac)
        ac->setAudioFileParameter(audioFileIndex, path);
}

void AserveAudio::audioFilePlayStateChanged(const int audioFileIndex, const AudioFilePlayerManager::PlayState state)
{
    AserveController* ac = AserveController::getInstance();
    if (ac)
        ac->setAudioFileParameter(audioFileIndex, static_cast<int> (state));
}