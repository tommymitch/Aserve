/*
 *  AserveWindow.cpp
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#include "AserveWindow.h"
#include "AserveComponent.h"

//==============================================================================
AserveWindow::AserveWindow()
:   
// Initialise the base 'DocumentWindow'...
DocumentWindow(
				T(PROJECT_NAME),				// Set the text to use for the title
				Colours::azure,					// Set the colour of the window
				DocumentWindow::allButtons,		// Set which buttons are displayed
				true							// This window should be added to the desktop
			  )
{
    setResizable(true, false); // resizability is a property of ResizableWindow, which is
	// a parent class of DocumentWindow (which is our base class),
	// so we have access to this setting here.
	
	setTitleBarHeight(20);      // Set the height of the titlebar on our window.
	setUsingNativeTitleBar (true);
	// create the app component, which is described in AppComponent.h
    AserveComponent* appComponent = new AserveComponent();
	
    // This sets the app content component for the window to be whatever AppComponent
    // is. The nature of DocumentWindow means that the contentComponent will fill the app
	// area of the window, and will be deleted automatically when the window is deleted.
    setContentOwned(appComponent, false);
    setMenuBar (appComponent);
}

AserveWindow::~AserveWindow()
{
    // Our 'content component' will get deleted by the destructor in the DialogWindow
	// base class, and that will, in turn (assuming the AppComponent has been coded
	// properly), clean up the other components contained inside it. Therefore, we have
	// nothing much to do here!
}

void AserveWindow::closeButtonPressed()
{
	// This is a virtual function provided by the DocumentWindow class, allowing us
	// to define the action taken when the window's 'close' button is pressed.
	
    // The correct thing to do when you want the app to quit is to call the
    // JUCEApplication::systemRequestedQuit() method.
	
    // That means that requests to quit that come from your own UI, or from other
    // OS-specific sources (e.g. the dock menu on the mac) all get handled in the
    // same way.
	
	// So, here, we get the application instance (from the static getInstance()
	// function in JUCEApplication), and call the function we need from it...
    JUCEApplication::getInstance()->systemRequestedQuit();
} 