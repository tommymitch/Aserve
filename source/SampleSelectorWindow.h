/*
 *  SampleSelectorWindow.h
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SAMPLESELECTORWINDOW__
#define __SAMPLESELECTORWINDOW__

#include <juce.h>
#include "AserveAudio.h"

class SampleSelectorComponent : public Component,
								public FilenameComponentListener,
								public ButtonListener
{
private:
	AserveAudio& aserveAudioMidi;// this is needed 
	int sampleCount;
	
	Array <Label*> labels;
	Array <FilenameComponent *> fileChooser;
	Array <TextButton*> playButton, stopButton;
	ActionBroadcaster audioBroadcaster;

public:
	SampleSelectorComponent(AserveAudio& aserveAudioMidi_);
	~SampleSelectorComponent();
	void updateButtons(void);
	void buttonClicked(Button* button);
	void filenameComponentChanged (FilenameComponent*);
	void resized();
	void loadFile(int fileNumber, String& path);
	
};

class SampleSelectorWindow  :   public DocumentWindow
{

private:
	SampleSelectorComponent *sampleSelectorComponent;
	
public:
	SampleSelectorWindow(AserveAudio& aserveAudioMidi_);
	~SampleSelectorWindow();
	void updateButtons(void);
	void loadFile(int fileNumber, String& path);
	void closeButtonPressed();
};


#endif   // __SAMPLESELECTORWINDOW__