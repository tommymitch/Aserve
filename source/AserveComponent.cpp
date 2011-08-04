/*
 *  AserveComponent.cpp
 *  jucedemo
 *
 *  Created by The Butcher on 18/09/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveComponent.h"

Aserve::Aserve()
{
	const float defaultVolume = 0.8f;
	
	setName (T("Audio"));

	//==============================================================================
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

  	addAndMakeVisible (volume_slider = new Slider (T("slider")));
	
	volume_slider->setRange (0.0, 1.0, 0.01);
	volume_slider->setPopupMenuEnabled (true);
	volume_slider->setValue (defaultVolume, false, false);
	volume_slider->setSliderStyle (Slider::Rotary);
	volume_slider->setRotaryParameters (float_Pi * 1.2f, float_Pi * 2.8f, false);
	volume_slider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
	volume_slider->addListener(this);
	
	addAndMakeVisible (aserveScope = new AserveScope());
	//server and GUI Stuff---------------------------------------
	
	aserveAudioMidi = new AserveAudio(aserveScope);
	server.addGuiActionListener(this);
	server.addAudioActionListener(aserveAudioMidi);
	
	aserveAudioMidi->addActionListener(this);
	aserveAudioMidi->addActionListener(&server);
	
	audioBroadcaster.addActionListener(aserveAudioMidi);
	commsBroadcaster.addActionListener(&server);
	
	addAndMakeVisible (messageBox = new TextEditor (T("messages")));
	messageBox->setReadOnly (true);
	messageBox->setMultiLine (true);
	
	addAndMakeVisible(audioSettingsButton = new TextButton (T("audio prefs"), T("click here to display the audio settings")));
	audioSettingsButton->addButtonListener (this);
	audioSettingsButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
	audioSettingsButton->setColour (TextButton::buttonOnColourId, Colours::lightgrey);
	audioSettingsButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnLeft);
	
	addAndMakeVisible(sequencerButton = new TextButton (T("seq"), T("click here to display the sequencer window")));
	sequencerButton->addButtonListener (this);
	sequencerButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
	sequencerButton->setColour (TextButton::buttonOnColourId, Colours::lightgrey);
	sequencerButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnLeft);
	
	addAndMakeVisible(fileButton = new TextButton (T("files"), T("click here to display the files window")));
	fileButton->addButtonListener (this);
	fileButton->setColour (TextButton::buttonColourId, Colours::darkgrey);
	fileButton->setColour (TextButton::buttonOnColourId, Colours::lightgrey);
	fileButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnLeft);
	
	
	open();

	//--------------------
	//initialise audio select sub-window
	sampleSelectorWindow = new SampleSelectorWindow(*aserveAudioMidi);
	sequencerWindow = new SequencerWindow(server);
	
}

Aserve::~Aserve()
{
	close();
	//delete server;
	
	delete aserveAudioMidi;
	delete sampleSelectorWindow;
	delete sequencerWindow;
	
	deleteAllChildren();
}

//==============================================================================
void Aserve::paint (Graphics& g)
{
	// print some text to explain what state we're in.

}

void Aserve::resized()
{
	//waveformComponent->setBounds (10, 10, getWidth() - 20, getHeight()*0.5);
	const int height = getHeight();
	const int width = getWidth();
	
	float buttonHeight = (float)(height-10) / 4.0f;
	
	aserveScope->setBounds(5, 5, (width-10) - 40, (int)((height-10)*0.5f));
	messageBox->setBounds (5, (int)((height-10)*0.5f)+5 , (width-10) - 40, (int)((height-10)*0.5f));
	
	volume_slider->setBounds((width-5) - 40, 5, 40, (int)buttonHeight);
	audioSettingsButton->setBounds((width-5) - 40, (int)(5+buttonHeight), 40, (int)buttonHeight);
	sequencerButton->setBounds((width-5) - 40, (int)(5+buttonHeight+buttonHeight), 40, (int)buttonHeight);
	fileButton->setBounds((width-5) - 40, (int)(5+buttonHeight+buttonHeight+buttonHeight), 40, (int)buttonHeight);
	

	updateButtons();
}

void Aserve::updateButtons()
{

	//repaint();
}

void Aserve::buttonClicked (Button* button)
{
	
	
		if(button == audioSettingsButton)
			aserveAudioMidi->showAudioSettings(this);
		else if(button == sequencerButton)
			showSequencerWindow();
		else if(button == fileButton)
			showSampleSelector();
}

void Aserve::actionListenerCallback(const String& message)
{
	parseMessage(message);
}

void Aserve::parseMessage(const String& message)
{
	String string = message;
	if(string.startsWith(T("sub")))
	{
		if(string.contains(T("updatebuttons")))//updatebuttons
		{
			sampleSelectorWindow->updateButtons();
		}
		else if(string.contains(T("load")))
		{
			String sampString = string.substring(string.indexOfChar(':')+1);//to load
			sampString = sampString.substring(sampString.indexOfChar(':')+1);// filenumber
			const int fileNumber = sampString.getIntValue();
			sampString = sampString.substring(sampString.indexOfChar(':')+1);// second
			if(inRange("Sample number", fileNumber, 0, 7))
			{
				sampleSelectorWindow->loadFile(fileNumber, sampString);
			}
		}
		else
		{
			String sampString = string.substring(string.indexOfChar(':')+1);//go from first number
			const int row = sampString.getIntValue();
			sampString = sampString.substring(sampString.indexOfChar(':')+1);// third
			const int value = sampString.getIntValue();
			if(inRange("Row number", row, 0, 7) && inRange("Sequence value", value, 0, 65535))
			{
				//set the values
				sequencerWindow->setButtons(row, value);
			}
			sequencerWindow->updateButtons();
		}
	}
	else if(string.startsWith(T("con")))	//connection has been opened/closed
	{
		//connection has opened or closed
		String conString = string.substring(string.indexOfChar(':')+1);//go from first number
		const int conOpen = conString.getIntValue();
		conString = conString.substring(conString.indexOfChar(':')+1);// third
		const int conNumber = conString.getIntValue();
		if(inRange("Connection open", conOpen, 0, 1))
		{
			sequencerWindow->reset();
			if(conOpen)
			{
				appendMessage(T("Connection #")+String(conNumber)+T(" - open"));
			}
			else
			{
				appendMessage(T("Connection #")+String(conNumber)+T(" - lost"));
				open();
			}
		}
	}
	else if(message.startsWith(T("midiraw")))
	{
		//do nothing these messages are for the comms object
	}
	else //all other messages are posted onto the gui text panel
	{
		appendMessage(message); 
	}
}

bool Aserve::inRange(const char *parameter, double value, double min, double max)
{
	//IMPORTANT - NEED TO FIND A CROSS PLATFORM METHOD FOR CHECKING FOR INF and NaN! as 
	//these may get through this check.
	bool inrange = false;
	(value > max || value < min) ? inrange = false : inrange = true;
	
	if(!inrange)
		appendMessage(String(parameter) + T(" Out of Range: ") + String(value) + T("  ")
							+ T("Range: ") + String(min) + T("-") + String(max));
	
	return inrange;
}

void Aserve::showSampleSelector()
{
	sampleSelectorWindow->centreAroundComponent (this, 500, (8+1)*25);
	sampleSelectorWindow->setVisible (true);
}

void Aserve::showSequencerWindow()
{
	sequencerWindow->centreAroundComponent (this, 600, (8+1)*25);
	sequencerWindow->setVisible (true);
}


void Aserve::sliderValueChanged (Slider* slider)
{
	if(slider == volume_slider)
	{
		double val = slider->getValue();
		audioBroadcaster.sendActionMessage(T("vol:")+String(val));//format volume command for AudioMidi object
	}
}

//Server Classes and Methods
//==============================================================================
// Just closes any connections that are currently open.
void Aserve::close()
{
	//server->stop();
	server.stop();
	//activeConnections.clear();
	
	// Reset the UI stuff to a disabled state.
	//messageBox->setText (String::empty, false);
	//messageBox->setVisible (true);
	
	appendMessage ("connection closed.");
}


void Aserve::open ()
{
	close();

	//messageBox->setText (String::empty, false);
	//messageBox->setVisible (true);
	
	// and try to open the socket or pipe...
	bool openedOk = false;
	
	// if we're starting up a server, we need to tell the server to start waiting for
	// clients to connect. It'll then create connection objects for us when clients arrive.

	//openedOk = server->beginWaitingForSocket (8000);
	openedOk = server.beginWaitingForSocket (8000);
			
	if (openedOk)
		appendMessage (T("Waiting for aserve requests..."));
	
		
	
	if (! openedOk)
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Interprocess Comms Demo"),
									 T("Failed to open the socket..."));
	}
}


void Aserve::appendMessage (const String& message)
{
	messageBox->setCaretPosition (INT_MAX);
	messageBox->insertTextAtCaret(message + T("\n"));
	messageBox->setCaretPosition (INT_MAX);
}

//==============================================================================
Component* createAudioDemo()
{
    return new Aserve();
}
