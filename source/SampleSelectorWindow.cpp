/*
 *  SampleSelectorWindow.cpp
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SampleSelectorWindow.h"

//----
SampleSelectorComponent::SampleSelectorComponent(AserveAudio& aserveAudioMidi_)
:	aserveAudioMidi(aserveAudioMidi_),
sampleCount(8)//number of possible samples
{
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	
	//add file choosers
	
	for(int i = 0; i<sampleCount;i++)
	{
		fileChooser.add(new FilenameComponent (T("audiofile ") + String(i),
											   File::nonexistent,
											   true, false, false,
											   formatManager.getWildcardForAllFormats(),
											   String::empty,
											   T("(choose a WAV or AIFF file)")));
		
		labels.add(new Label(T("label")+ String(i), T("Sample ") + String(i+1)));
		
		playButton.add(new TextButton (T("play"), T("click here to play the current audio file")));
		playButton[i]->addButtonListener (this);
		playButton[i]->setColour (TextButton::buttonColourId, Colours::white);
		playButton[i]->setColour (TextButton::buttonOnColourId, Colours::white);
		playButton[i]->setConnectedEdges (Button::ConnectedOnRight);
		
		stopButton.add(new TextButton (T("stop"), T("click here to stop the current audio file")));
		stopButton[i]->addButtonListener (this);
		stopButton[i]->setColour (TextButton::buttonColourId, Colours::lightblue);
		stopButton[i]->setColour (TextButton::buttonOnColourId, Colours::lightblue);
		stopButton[i]->setConnectedEdges (Button::ConnectedOnLeft);
		
		addAndMakeVisible (labels[i]);
		addAndMakeVisible (fileChooser[i]);
		addAndMakeVisible (playButton[i]);
		addAndMakeVisible (stopButton[i]);
		
		fileChooser[i]->addListener (this);
		fileChooser[i]->setBrowseButtonText (T("browse"));
	}
	
	updateButtons();
	audioBroadcaster.addActionListener(&aserveAudioMidi);
}

SampleSelectorComponent::~SampleSelectorComponent()
{
	// the text editor gets deleted automatically because it's the
	// window's content component.
	deleteAllChildren();	
	//sourceWindow = 0;
}	

void SampleSelectorComponent::filenameComponentChanged (FilenameComponent* filenameComponent)
{
	// this is called when the user changes the filename in the file chooser box
	//printf("%s\n",(const char *)filenameComponent->getCurrentFile().getFullPathName());
	for(int i = sampleCount;--i>=0;)
	{
		if(filenameComponent == fileChooser[i]) //needs to be an internal audio broardcaster that the audio object listens to
			audioBroadcaster.sendActionMessage(T("samp:")+String(LOADFILE)+T(":")+String(i)+T(":")//tell the audiomidi object to load the file
													 +filenameComponent->getCurrentFile().getFullPathName());
		//maybe you should do this by calling loadSample in the AudioMidi object rather than action message stylee
		//owner.loadSample(filenameComponent,i);
	}
	
	updateButtons();//this will be too soon so you'll have to triggger this from the Audio thread
}

void SampleSelectorComponent::updateButtons()
{
	for(int i = sampleCount; --i>=0;)
	{
		//playButton[i]->setEnabled (owner.currentAudioFileSource[i] != 0 && ! owner.transportSource[i].isPlaying());
		//stopButton[i]->setEnabled (owner.transportSource[i].isPlaying());
		playButton[i]->setEnabled (aserveAudioMidi.fileLoaded(i) && ! aserveAudioMidi.filePlaying(i));
		stopButton[i]->setEnabled (aserveAudioMidi.filePlaying(i));
		//I don't like these functions as they mean we need access to the audio object here (rather than the broardcast method)
	}
	repaint();
}

void SampleSelectorComponent::buttonClicked (Button* button)
{
	for(int i = sampleCount; --i>=0;)
	{
		if (button == playButton[i])
		{
			audioBroadcaster.sendActionMessage(T("samp:")+String(PLAYFILE)+T(":")+String(i)+T(":")+String(1.0));
			//owner.transportSource[i].setPosition (0.0);
			//owner.transportSource[i].start();
		}
		else if (button == stopButton[i])
		{
			audioBroadcaster.sendActionMessage(T("samp:")+String(PLAYFILE)+T(":")+String(i)+T(":")+String(0.0));//zero amplitude stops playback
			//owner.transportSource[i].stop();
		}
	}
	//updateButtons(); this is triggered by the AudioMidi object when action has been implemented
}

void SampleSelectorComponent::resized()
{
	float height = (getHeight()-20)/8.0f; 
	for(int i = sampleCount; --i>=0;)
	{
		labels[i]->setBounds(10, (int)(10+i*height), 60, (int)height-1);
		fileChooser[i]->setBounds (70, (int)(10+i*height), getWidth() - 164, (int)height-1);
		playButton[i]->setBounds (getWidth() - 90, (int)(10+i*height), 40, (int)height-1);
		stopButton[i]->setBounds (getWidth() - 50, (int)(10+i*height), 40, (int)height-1);
	}
	
	
	//updateButtons();
}	

void SampleSelectorComponent::loadFile(int fileNumber, String& path)
{
	fileChooser[fileNumber]->setCurrentFile(File(path),true, true);
}

SampleSelectorWindow::SampleSelectorWindow(AserveAudio& aserveAudioMidi_):
DocumentWindow (T("Sample Selector Window"), Colours::lightgrey,  DocumentWindow::allButtons, true)
{
	//setUsingNativeTitleBar(true);	
	setResizable (true, false);		

	sampleSelectorComponent = new SampleSelectorComponent(aserveAudioMidi_);
	setContentComponent(sampleSelectorComponent); 
}

SampleSelectorWindow::~SampleSelectorWindow()
{
	setContentComponent (0, true);
	//delete sampleSelectorComponent;
}

void SampleSelectorWindow::updateButtons(void)
{
	sampleSelectorComponent->updateButtons();
}

void SampleSelectorWindow::closeButtonPressed()
{
	
	setVisible(false);
	//delete this;
}

void SampleSelectorWindow::loadFile(int fileNumber, String& path)
{
	sampleSelectorComponent->loadFile(fileNumber, path);
}

