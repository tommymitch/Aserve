/*
 *  ErrorTextBox.cpp
 *  Aserve
 *
 *  Created by tjmitche on 17/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ErrorTextBox.h"

ErrorTextBox::ErrorTextBox()
{
    textbox = new TextEditor();
    textbox->setMultiLine(true);
    textbox->setReadOnly(true);
    textbox->setCaretVisible(false);
    //setScrollbarsShown (bool shouldBeEnabled);
    //void setPopupMenuEnabled (bool menuEnabled);
    //void setScrollBarThickness (int newThicknessPixels);
    
    textbox->setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
    textbox->setColour (TextEditor::textColourId, Colours::red);
    
    addAndMakeVisible(textbox);
}

ErrorTextBox::~ErrorTextBox()
{

}

void ErrorTextBox::addErrorMessage(const String& message)
{
    postMessage(new ErrorMessage(message));
}

void ErrorTextBox::resized()
{
    textbox->setBounds(getLocalBounds());
}

void ErrorTextBox::handleMessage (const Message& message)
{
    const ErrorMessage& errMessage = dynamic_cast<const ErrorMessage&> (message);
    textbox->insertTextAtCaret(errMessage.error);
}