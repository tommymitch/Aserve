/*
 *  AserveApp.cpp
 *
 *  Created by tjmitche on 05/09/2011.
 *  Copyright 2011. All rights reserved.
 *
 */

#include "AserveApp.h"

AserveApp::AserveApp() : aserveWindow (0)
{

}

AserveApp::~AserveApp()
{

}

//==============================================================================
void AserveApp::initialise(const String& commandLine)
{
	aserveWindow = new AserveWindow();
	aserveWindow->centreWithSize(600, 620);   
	aserveWindow->setVisible(true);
}

void AserveApp::shutdown()
{
	deleteAndZero (aserveWindow);
}

//==============================================================================
void AserveApp::systemRequestedQuit()
{
	quit();
}

//==============================================================================
const String AserveApp::getApplicationName()
{
	// The name for the application
	return PROJECT_NAME;
}

const String AserveApp::getApplicationVersion()
{	
	return "1.4";
}

bool AserveApp::moreThanOneInstanceAllowed()
{
	return false;
}

void AserveApp::anotherInstanceStarted(const String& commandLine)
{
	
}

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(AserveApp)