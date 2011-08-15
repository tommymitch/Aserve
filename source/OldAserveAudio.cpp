/*
 *  AserveAudio.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OldAserveAudio.h"

OldAserveAudio::OldAserveAudio(AserveScope *aserveScopePtr)
{
	const float defaultVolume = 0.8f;

	for(int i = 8; --i>=0;)
		currentAudioFileSource[i] = 0;
	
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	
	//==============================================================================
	// register for start/stop messages from the transport source..
	for(int i = 8; --i>=0;)
		transportSource[i].addChangeListener (this);
	
	// and initialise the device manager with no settings so that it picks a
	// default device to use.
	const String error (audioDeviceManager.initialise (1, /* number of input channels */
													   2, /* number of output channels */
													   0, /* no XML settings.. */
													   true  /* select default device on failure */));
	
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Aserve"),
									 T("Couldn't open an output device!\n\n") + error);
	}
	else
	{
		// add the audio sources to our mixer..
		for(int i = 8; --i>=0;)
			mixerSource.addInputSource (&transportSource[i], false);
		mixerSource.addInputSource (&synthSource, false);
		//mixerSource.addInputSource (&toneSource, false);
		mixerSource.addInputSource(&oscillatorSource, false);
		
		//toneSource.setFrequency(1000);
		//toneSource.setAmplitude(0.0);
		for (int oscillatorNumber = OSCILLATOR_COUNT; --oscillatorNumber >= 0;)
		{
			oscillatorSource.setFrequency(oscillatorNumber, 1000);
			oscillatorSource.setAmplitude(oscillatorNumber, 0.0);
		}
		
		// ..and connect the mixer to our source player.
		audioSourcePlayer.setSource (&mixerSource);
		audioSourcePlayer.setGain(defaultVolume);
		
		//link the scope with the audio i/o
		aserveScope = aserveScopePtr;
		// start the IO device pulling its data from our callback..
		audioDeviceManager.addAudioCallback (this);
		
		// and we need to send midi input to our synth for processing
		//audioDeviceManager.addMidiInputCallback (String::empty, &synthSource.midiCollector);
		// we also need to pump the midi down the socket
		audioDeviceManager.addMidiInputCallback (String::empty, this);
		
		//Logger::outputDebugString(MidiInput::getDevices());
		StringArray s(MidiInput::getDevices());
		audioDeviceManager.setMidiInputEnabled(s[0],true);
		s = MidiOutput::getDevices();
		audioDeviceManager.setDefaultMidiOutput(s[0]);
	}
}
OldAserveAudio::~OldAserveAudio()
{
	audioDeviceManager.removeMidiInputCallback (String::empty,&synthSource.midiCollector);
	audioDeviceManager.removeAudioCallback (this);
	
	for(int i = 8; --i>=0;)
	{
		transportSource[i].removeChangeListener (this);
		transportSource[i].setSource (0);
		deleteAndZero (currentAudioFileSource[i]);
		audioSourcePlayer.setSource (0);
	}
	//deleteAllChildren(); //does the audio object have any children?
	
}

void OldAserveAudio::audioDeviceIOCallback (const float** inputChannelData,
										 int totalNumInputChannels,
										 float** outputChannelData,
										 int totalNumOutputChannels,
										 int numSamples)
{
	// pass the audio callback on to our player source, and also the waveform display comp
	audioSourcePlayer.audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
	aserveScope->audioDeviceIOCallback ((const float **)outputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
	//
}

void OldAserveAudio::audioDeviceAboutToStart (AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart (device);
	aserveScope->audioDeviceAboutToStart (device);
}

void OldAserveAudio::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
	aserveScope->audioDeviceStopped();
}

void OldAserveAudio::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message)
{
	
	unsigned char *rawData;	
	rawData = message.getRawData();
	
	String srawData(T("midiraw"));
	for(int i = 0; i < message.getRawDataSize();i++)
	{
		srawData = srawData + T(":") + String(*rawData);
		rawData++;
	}
	
	if(message.getRawDataSize() == 2)		//if only one databyte pad the other
		srawData = srawData + T(":") + String(-1);
	
	//printf("%s\n",(const char *)srawData);
	
	{
		//printf("AserveAudioMidiThread - %d (?) ActionMessageGenerated\n", Thread::getCurrentThreadId());
		sendActionMessage(srawData);	//this should broardcast the midi data to the InterProcessConnection object where it will be transmitted(hopefully)
		//const MessageManagerLock mmLock;//this locks the message thread try to do this through some other communications method change broardcaster.
		//owner.sendSocketMessage(srawData); -> need to post the data down the socket synchronously or as soon as possible
		//owner.appendMessage(srawData);
	}	
	
}

void OldAserveAudio::changeListenerCallback (ChangeBroadcaster* source)//this should broardcast to the gui object
{												
	// callback from the transport source to tell us that play has
	// started or stopped, so update our buttons..
	//sampleSelectorWindow->updateButtons();
	sendActionMessage(T("sub:updatebuttons"));//updateButtons();  //need to post this to gui!
}

void OldAserveAudio::addMidiActionListener(ActionListener* const listener)
{
	addActionListener(listener);
}

void OldAserveAudio::actionListenerCallback(const String& message)
{
	//printf("AserveAudio Recieved ActionMessage::%s\n", (const char *)message);
	parseMessage(message);
}

void OldAserveAudio::parseMessage(const String& message)
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
		
		if(inRange("Osc Number", oscillatorNumber, 0, 15) && inRange("Osc Freq", freq, 0, 20000) 
		   && inRange("Osc Amp", amp, 0, 1) && inRange("Osc Wave", wave, 0, 2))
		{
			//NEED SYNC(?) CONTROL OVER THE AUDIO FUNCTIONS
			if(amp > 0.0)
				oscillatorSource.setAmplitude(oscillatorNumber,amp);
			 else
				 oscillatorSource.stop(oscillatorNumber);
			 
			 oscillatorSource.setFrequency(oscillatorNumber,freq);
			 oscillatorSource.setWave(oscillatorNumber,wave);
		}
		
	}
	else if(string.startsWith(T("note")))
	{
		String oscString = string.substring(string.indexOfChar(':')+1);//go from first number
		const int wave = oscString.getIntValue();
		oscString = oscString.substring(oscString.indexOfChar(':')+1);//from the second num
		const int note = oscString.getIntValue();
		oscString = oscString.substring(oscString.indexOfChar(':')+1);// third
		const int vel = oscString.getIntValue();
		
		//owner.appendMessage(String(note)+T(":")+String(wave)+T(":")+String(vel));
		
		if(inRange("Note Wave", wave, 0, 3) && inRange("Note Note", note, 0, 127) && inRange("Note Velocity", vel, 0, 127))
		{
			if(!vel)
				synthSource.noteOff(1,note,true);
			else
				synthSource.noteOnWave(wave,1,note,vel/127.0f);	//play a note*/
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
					transportSource[sampleNumber].setPosition (0.0);
					transportSource[sampleNumber].setGain(gain);
					transportSource[sampleNumber].start();
				}
				else
				{
					transportSource[sampleNumber].stop();
				}
			}
		}
		else if(sampleCommand == LOADFILE)
		{
			const int sampleNumber = sampString.getIntValue();
			sampString = sampString.substring(sampString.indexOfChar(':')+1);// third should be the path
			if(inRange("Sample number", sampleNumber, 0, 7))
			{
				loadSample(sampleNumber, sampString);//line below triggers this
				//sendActionMessage(T("seq:load:")+String(sampleNumber)+T(":")+sampString);
			}
		}
		//both LOADFILE and PLAYFILE require the spamle window buttons updating
		/*owner.sampleSelectorWindow->updateButtons();*///post this to the gui object
		sendActionMessage(T("sub:updatebuttons"));//updateButtons();  //need to post this to gui!
		
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

bool OldAserveAudio::inRange(const char *parameter, double value, double min, double max)
{
	//IMPORTANT - NEED TO FIND A CROSS PLATFORM METHOD FOR CHECKING FOR INF and NaN! as 
	//these may get through this check.
	bool inrange = false;
	(value > max || value < min) ? inrange = false : inrange = true;
	
	if(!inrange)
		sendActionMessage(String(parameter) + T(" Out of Range: ") + String(value) + T("  ")
					  + T("Range: ") + String(min) + T("-") + String(max));
	
	return inrange;
}

void OldAserveAudio::stopAll()
{
	synthSource.allNotesOff();
	//owner.toneSource.setAmplitude(0);
	oscillatorSource.stopAll();
	//stop all samples too
	for(int i = 8; --i >=0; )
		transportSource[i].stop();
}

void OldAserveAudio::loadSample (int fileIndex, String& filePath)
{
	// this is called when the user changes the filename in the file chooser box
	File audioFile (filePath);

	if(audioFile.existsAsFile())
	{
		
		// unload the previous file source and delete it..
		transportSource[fileIndex].stop();
		transportSource[fileIndex].setSource (0);
		deleteAndZero (currentAudioFileSource[fileIndex]);
	
		// create a new file source from the file..
	
		// get a format manager and set it up with the basic types (wav and aiff).
		AudioFormatManager formatManager;
		formatManager.registerBasicFormats();
	
		AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
	
		if (reader != 0)
		{
			currentFile[fileIndex] = audioFile;
		
			currentAudioFileSource[fileIndex] = new AudioFormatReaderSource (reader, true);
		
			// ..and plug it into our transport source
			transportSource[fileIndex].setSource (currentAudioFileSource[fileIndex],
											  32768, // tells it to buffer this many samples ahead
											  reader->sampleRate);
		}
	}
	else
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "Aserve: File Not Found",
									 "The requested file: " + filePath + " could not be found. Check the file path for errors.",
									 "ok");
		//some message on the GUI that says the file doesn't exist
	}
}

bool OldAserveAudio::fileLoaded(int sampleNumber)
{
	return 	currentAudioFileSource[sampleNumber] != 0;
}

bool OldAserveAudio::filePlaying(int sampleNumber)
{
	return transportSource[sampleNumber].isPlaying();
}

void OldAserveAudio::showAudioSettings(Component *centerComp)
{
	AudioDeviceSelectorComponent audioSettingsComp (audioDeviceManager,
													0, 0,
													2, 2,
													true, true,
													true, true);
	
	// ...and show it in a DialogWindow...
	audioSettingsComp.setSize (500, 250);
	
	
	DialogWindow::showModalDialog (T("Audio Settings"),
								   &audioSettingsComp,
								   centerComp,
								   Colours::grey,
								   true);
	
}