/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef ASERVECOMPONENT_H_INCLUDED
#define ASERVECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Audio.h"
#include "AudioLiveScrollingDisplay.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AserveComponent   : public Component
{
public:
    //==============================================================================
    AserveComponent (Audio& a);
    ~AserveComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    Audio& audio;
    LiveScrollingAudioDisplay scope;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AserveComponent)
};


#endif  // ASERVECOMPONENT_H_INCLUDED
