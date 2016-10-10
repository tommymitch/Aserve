/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "AserveComponent.h"


//==============================================================================
AserveComponent::AserveComponent (Audio& a) : audio (a)
{
    audio.addAudioCallback (&scope);
    
    setSize (400, 200);
    addAndMakeVisible (scope);
}

AserveComponent::~AserveComponent()
{
    audio.removeAudioCallback (&scope);
}

void AserveComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    scope.setBounds (getLocalBounds());
}

void AserveComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
