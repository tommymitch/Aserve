/*
 *  AserveModel.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ASERVEMODEL
#define H_ASERVEMODEL

#include "../../JuceLibraryCode/JuceHeader.h"

/**
 Top level of the model class where all internal data is stored
 */
class AserveModel  
{
public:
    /**
     Gui constructor
     */
    AserveModel();
    /**
     Gui destructor
     */
    ~AserveModel();
    
    /**
     Returns the value tree object
     */
	ValueTree* getValueTree()	{	return &valueTree;	}
private:
    ValueTree valueTree;    //where all the values are stored
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveModel)
};  


#endif //H_ASERVEMODEL