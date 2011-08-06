/*
 *  AserveWindow.h
 *
 *  Created by tjmitche 05/09/2011.
 *  All rights reserved.
 *
 */

#ifndef H_ASERVEWINDOW
#define H_ASERVEWINDOW

#include <juce.h>

class AserveWindow  : public DocumentWindow
{
public:
	//==============================================================================
	AserveWindow();
	~AserveWindow();
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void closeButtonPressed();
	
};



#endif //H_ASERVEWINDOW