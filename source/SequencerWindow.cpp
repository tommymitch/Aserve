/*
 *  SequencerWindow.cpp
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SequencerWindow.h"

SequencerComponent::SequencerComponent(AserveInterprocessConnectionServer& server)
:	rows(8), 
columns(16)
{	
	//add file choosers
	
	for(int y = 0; y < rows; y++)
	{
		sequencerButtons.add(new Array <TextButton*>);
		labels.add(new Label(T("label")+ String(y), String(y)));
		addAndMakeVisible (labels[y]);
		for(int x = 0; x < columns; x++)
		{
			
			
			sequencerButtons[y]->add(new TextButton (T("0"), T("click to toggle")));
			(*sequencerButtons[y])[x]->addButtonListener (this);
			(*sequencerButtons[y])[x]->setColour (TextButton::buttonColourId, Colours::white);
			(*sequencerButtons[y])[x]->setColour (TextButton::buttonOnColourId, Colours::grey);
			(*sequencerButtons[y])[x]->setClickingTogglesState(true);
			
			sequencerBitState.add(new BitArray);
			
			if(x == 0)
				(*sequencerButtons[y])[x]->setConnectedEdges ( Button::ConnectedOnLeft );
			else if(x == (columns-1))
				(*sequencerButtons[y])[x]->setConnectedEdges ( Button::ConnectedOnRight );
			else
				(*sequencerButtons[y])[x]->setConnectedEdges ( Button::ConnectedOnRight | Button::ConnectedOnLeft );
			
			addAndMakeVisible ((*sequencerButtons[y])[x]);
		}
		
	}
	
	updateButtons();
	
	commsBroadcaster.addActionListener(&server);
}

SequencerComponent::~SequencerComponent()
{
	// the text editor gets deleted automatically because it's the
	// window's content component.
	deleteAllChildren();	
	//sourceWindow = 0;
}	



void SequencerComponent::updateButtons()
{
	
	repaint();
}

void SequencerComponent::setButtons(int row, int val)
{
	sequencerBitState[row]->setBitRangeAsInt(0,16,val);
	
	for(int x = 0; x < columns; x++)
	{														//cast to bool
		(*sequencerButtons[row])[x]->setToggleState((sequencerBitState[row]->getBitRangeAsInt(x,1)) != 0, false);//cant broardcast a click as this would overwrite the bitstring
		updateButtonText((*sequencerButtons[row])[x]);
	}
	
	String string(T("sequence"));
	string = string + T(":") + String(row) + T(":") + String(sequencerBitState[row]->getBitRangeAsInt(0,16));
	//printf("%s\n",(const char *) string);
	//const MessageManagerLock mmLock;//this locks the message thread
	commsBroadcaster.sendActionMessage(string);
	//owner.sendSocketMessage(string);
	//owner.appendMessage(string);
	
	repaint();
}

void SequencerComponent::updateButtonText(Button* button)
{
	if(button->getToggleState())
		button->setButtonText("1");
	else
		button->setButtonText("0");
}

void SequencerComponent::buttonClicked (Button* button)
{
	
	updateButtonText(button);
	
	for(int y = 0; y < rows; y++)
	{
		for(int x = 0; x < columns; x++)
		{
			if (button == (*sequencerButtons[y])[x])
			{
				sequencerBitState[y]->setBit(x,button->getToggleState());
				
				String string(T("sequence"));
				string = string + T(":") + String(y) + T(":") + String(sequencerBitState[y]->getBitRangeAsInt(0,16));
				
				//const MessageManagerLock mmLock;//this locks the message thread
				//owner.sendSocketMessage(string);
				//owner.appendMessage(string);
				//printf("%s\n",(const char *) string);
				commsBroadcaster.sendActionMessage(string);
			}
		}
	}
	
	updateButtons();
}

void SequencerComponent::reset (void)
{
	for(int y = 0; y < rows; y++)
		for(int x = 0; x < columns; x++)
			(*sequencerButtons[y])[x]->setToggleState(false,true);	
}

void SequencerComponent::resized()
{
	
	buttonWidth = (float)(getWidth() - 40) / columns;
	buttonHeight = ((float)(getHeight() - 20) / rows) -3.0f;
	
	for(int y = 0; y < rows; y++)
	{
		labels[y]->setBounds(10, (int)(10 + y*(buttonHeight+3)), 20, (int)buttonHeight);
		for(int x = 0; x < columns; x++)
		{
			(*sequencerButtons[y])[x]->setBounds(getWidth() - (int)(10 + (x+1)*buttonWidth), (int)(10 + y*(buttonHeight+3)), (int)buttonWidth, (int)buttonHeight);
		}
	}
	
	//updateButtons();
}	

SequencerWindow::SequencerWindow(AserveInterprocessConnectionServer& server)
:	DocumentWindow (T("Sequencer Window"), Colours::lightgrey,  DocumentWindow::allButtons, true)
{	
	//setUsingNativeTitleBar(true);	
	setResizable (true, false);		
	
	sequencerComponent = new SequencerComponent(server);
	setContentComponent(sequencerComponent); 	
}


SequencerWindow::~SequencerWindow()
{
	//delete sequencerComponent;
	setContentComponent (0, true);
	//delete sequencerComponent;
	
}

void SequencerWindow::setButtons(int row, int value)
{
	sequencerComponent->setButtons(row, value);
}

void SequencerWindow::updateButtons(void)
{
	sequencerComponent->updateButtons();
}

void SequencerWindow::reset(void)
{
	sequencerComponent->reset();
}

void SequencerWindow::closeButtonPressed()
{
	
	setVisible(false);
	//delete this;
}

/*void SequencerWindow::positionWindow(Component *_owner)
{
	centreAroundComponent ( _owner, columns * buttonWidth +20, 
						   buttonWidth*rows+20 );
	
}*/
