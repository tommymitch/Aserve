/*
 *  ErrorTextBox.h
 *  Aserve
 *
 *  Created by tjmitche on 17/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ERRORTEXTBOX
#define H_ERRORTEXTBOX

#include "../../JuceLibraryCode/JuceHeader.h"

/**
 Top level of all gui objects
 */
class ErrorTextBox  :    public Component,
                         public MessageListener
{
public:
    /**
     ErrorTextBox constructor
     */
    ErrorTextBox();
    /**
     ErrorTextBox destructor
     */
    ~ErrorTextBox();
    
    /**
     Inserts the string to the textbox
     */
    void addErrorMessage(const String& message);
    
    //Component Callbacks=====================================================================
    void resized();

    //message==========================================================================
    void handleMessage (const Message& message);
    
private:    
    ScopedPointer<TextEditor> textbox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ErrorTextBox)
};

class ErrorMessage : public Message
{
public:
    ErrorMessage(const String error_)  
    : error(error_) {}
    const String error;
};

#endif //H_ERRORTEXTBOX