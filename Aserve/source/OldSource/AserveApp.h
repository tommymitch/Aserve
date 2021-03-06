/*
 *  AserveApp.h
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#ifndef H_ASERVEAPP
#define H_ASERVEAPP

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveWindow.h"

//==============================================================================
class AserveApp  : public JUCEApplication
{
public:
    //==========================================================================
    AserveApp();
    ~AserveApp();
    //==========================================================================
    void initialise (const String& commandLine);
    void shutdown();
    //==========================================================================
    void systemRequestedQuit();
    //==========================================================================
    const String getApplicationName();
    const String getApplicationVersion();
    bool moreThanOneInstanceAllowed();
    void anotherInstanceStarted(const String& commandLine);
	
private:
    AserveWindow* aserveWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveApp)
};


#endif //H_ASERVEAPP