/*
 *  BitwiseSelector.h
 *  Aserve
 *
 *  Created by tjmitche on 15/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_BITWISESELECTOR
#define H_BITWISESELECTOR

#include "../../JuceLibraryCode/JuceHeader.h"

/**
 Top level of all gui objects
 */
class BitwiseSelector  :    public Component,
                            public Button::Listener,
                            public MessageListener
{
public:
    enum Constants 
    {
        NumBits = 16
    };
    /**
     BitwiseSelector constructor
     */
    BitwiseSelector();
    /**
     BitwiseSelector destructor
     */
    ~BitwiseSelector();
    
    /**
     sets the selector to the specified value
     */
    void setValue(int newValue);
    
    //Listener=====================================================================
    class Listener
    {
    public:
        /**
         Destructor
         */
        virtual ~Listener() {}
        
        /**
         one of the buttons is pressed or a file is loaded
         */
        virtual void bitwiseSelectorClicked(const BitwiseSelector* selector, const int bitNumber)=0;
    };
    
    /**
	 Adds a listener to recieve glove data
	 */
	void addListener(Listener *listenerToAdd);
	
    /**
	 Removes a listener
	 */
	void removeListener(Listener *listenerToRemove);
    
    
    //Component Callbacks=====================================================================
    void resized();
    //Button Callback
    void buttonClicked(Button* button);
    //Message Listener
    void handleMessage (const Message& message);
    //==========================================================================
    
private:    
    OwnedArray<TextButton> bits;
    
    //Listener
    ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitwiseSelector)
};

class BitwiseMessage : public Message
{
public:
    BitwiseMessage(const int bitsValue_) 
    : bitsValue(bitsValue_) {}
    const int bitsValue;
};

#endif //H_BITWISESELECTOR