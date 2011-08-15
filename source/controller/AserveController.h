/*
 *  AserveController.h
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ASERVECONTROLLER
#define H_ASERVECONTROLLER

#include <juce.h>
#include "AserveModel.h"
#include "AserveAudio.h"
#include "AserveNetwork.h"
#include "AserveGui.h"

/**
 Controller class all interactions between the toplevel objects are managed
 */
class AserveController
{
public:
    
    /**
     The same functions are used by the controlled and controlling objects this source
     index lets us know who is doing the controlling.
     */
    enum Sources 
    {
        Gui,
        Audio,
        Network,
        
        NumSources
    };
    
    /**
     This is a singleton class
     */
    juce_DeclareSingleton(AserveController, true);
    
    /**
     Gui constructor
     */
    AserveController();
    
    /**
     Gui destructor
     */
    ~AserveController();
    
    /**
     Internal storage for connected objects
     */
    void setModelObject(AserveModel* modelObject) { model = modelObject; }
    void setAudioObject(AserveAudio* audioObject) { audio = audioObject; }
    void setNetworkObject(AserveNetwork* networkObject) { network = networkObject; }

    //Model
    /**
     Common interface for getting model parameters.
     */
    const var& getModelParameter(const Identifier& group, const Identifier& parameter) const;
    
    /**
     Common interface for setting the model parameters.
     */
    void setModelParameter(const Identifier& group, const Identifier& parameter, const var& newValue);
    
    /**
     Sets the file name for the AudioFiles gui
     */
    void setAudioFileParameter(const int fileIndex, const String& path);
    
    /**
     Sets the play state for the AudioFiles gui -1 is disabled 0 is stop and 1 is play.
     */
    void setAudioFileParameter(const int fileIndex, const int playState);
    
    //Audio
    /**
     Stops all oscillators and files from playing
     */
    void stopAllAudio();
    
    /**
     handles the audio file play control from the gui to control the audio and from the 
     audio to control the gui (via the value tree)
     */
    void audioFile(const Sources source, const int fileIndex);
    
    /**
     handles the audio file name control from the gui to control the files loaded by the audio object and from the 
     audio object to control the gui (via the value tree)
     */
    void audioFile(const Sources source, const int fileIndex, const String &newFilePath);
    
    //Network
    /**
     enables network messages to be sent from anywhere
     */
    void sendNetworkMessage(const String& message);
    
    /**
     Parses the network messages and distributes commands acordingly
     */
    void parseNetworkMessage(const String& message);
    
private:
    //pointers to the external objects controlled through here
    AserveModel *model;
    AserveAudio *audio;
    AserveNetwork *network;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AserveController)
};  


#endif //H_ASERVECONTROLLER