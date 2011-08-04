/*
 *  SequencerWindow.h
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SEQUENCERWINDOW__
#define __SEQUENCERWINDOW__

#include <juce.h>
#include "AserveInterProcessConnection.h"

class SequencerComponent : public Component,
public ButtonListener
{
private:
	int rows, columns;
	float buttonWidth, buttonHeight;
	
	Array <Label*> labels;
	Array < Array < TextButton* > * > sequencerButtons;
	Array < BitArray *> sequencerBitState;
	ActionBroadcaster commsBroadcaster;
	
public:
	SequencerComponent(AserveInterprocessConnectionServer& server);
	~SequencerComponent();
	void updateButtons(void);
	void updateButtonText(Button* button);
	void setButtons(int row, int val);
	void buttonClicked(Button* button);
	void reset (void);
	void resized();
	
	
};

class SequencerWindow  :	public DocumentWindow

{
private:
	SequencerComponent *sequencerComponent;
	
public:
	SequencerWindow(AserveInterprocessConnectionServer& server);
	~SequencerWindow();
	
	void setButtons(int row, int value);
	void updateButtons(void);
	void reset();
	
	//void positionWindow(Component *_owner);
	void closeButtonPressed();
};

#endif   // __SEQUENCERWINDOW__