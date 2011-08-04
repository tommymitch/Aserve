/*
 *  AserveComponent.h
 *  jucedemo
 *
 *  Created by The Butcher on 18/09/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __ASERVE_COMPONENT__
#define __ASERVE_COMPONENT__

#include <juce.h>
#include "AserveClasses.h"
#include "MidiMuncher.h"
#include "AserveInterProcessConnection.h"
#include "AserveAudio.h"
#include "AserveScope.h"
#include "SampleSelectorWindow.h"
#include "SequencerWindow.h"



//==============================================================================
class Aserve  :	   public Component,
                   public ButtonListener,
				   public SliderListener,
				   public ActionListener	//To recieve GUI messages from interprocess and audioMidi objects
											//to post messages to the Audio Thread
{
    //==============================================================================

public:
    //==============================================================================
    Aserve();
    ~Aserve();
    //==============================================================================
    void paint (Graphics& g);
    void resized();
    void updateButtons();
    void buttonClicked (Button* button);
	void sliderValueChanged (Slider* slider);
	void actionListenerCallback(const String& message);
	void parseMessage(const String& message);
	bool inRange(const char *parameter, double value, double min, double max);

	//Server Classes and Methods
	//--------------------------------------------------
	//==============================================================================
    // Just closes any connections that are currently open.
	void close();
	void open ();
	void appendMessage (const String& message);
	
	/////////////////////////////////////////////
	//subwindow functions
	void showSampleSelector(void);
	void showSequencerWindow(void);

private:
	FilenameComponent* fileChooser;
	TextButton* sequencerButton;
	TextButton* fileButton;
	TextButton* audioSettingsButton;

	Slider* volume_slider;

	AserveScope *aserveScope;
	
	ActionBroadcaster commsBroadcaster,audioBroadcaster;

	//==============================================================================
	// Create the audioMidi object
	AserveAudio *aserveAudioMidi;
	
	//server stuff
	TextEditor* messageBox;
	//to deal with connections to uwe audioserver clients.
	AserveInterprocessConnectionServer server;

	
	
	/////////////////////////////
	//subwindows	
	SampleSelectorWindow *sampleSelectorWindow;
	SequencerWindow *sequencerWindow;

};

#endif   // __ASERVE_COMPONENT__