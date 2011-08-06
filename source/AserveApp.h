/*
 *  AserveApp.h
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#ifndef H_ASERVEAPP
#define H_ASERVEAPP

#include <juce.h>
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
};


#endif //H_ASERVEAPP