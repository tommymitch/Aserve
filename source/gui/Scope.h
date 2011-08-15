/*
 *  Scope.h
 *  Aserve
 *
 *  Created by tjmitche on 11/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_SCOPE
#define H_SCOPE

#include <juce.h>

/**
 Top level of all gui objects
 */
class Scope  :  public Component,
                public Timer,
                public AudioIODeviceCallback
{
public:
    /**
     Gui constructor
     */
    Scope();
    /**
     Gui destructor
     */
    ~Scope();
    
    void addSample (const float sample);
    void processAudio();
    void changeListenerCallback (void *objectThatHasChanged);
    
    //callbacks==========================================================================
    void paint (Graphics& g);
    void resized();
    void timerCallback();

    /**
     This fundtion takes audio data and is called synchronosly from the audio thread
     and adds it to the audioSampleBuffer when the buffer is full it is coppied to a 
     display buffer which is then processed and repainted asyncronously, the buffer is 
     then overwritten.
     */
    void audioDeviceAboutToStart (AudioIODevice*);
    void audioDeviceStopped ();
    void audioDeviceIOCallback (const float** inAudio,int numInChannels,float** outAudio,int numOutAudio,int numSamples);
    
    //==========================================================================
    
private:
    AudioSampleBuffer *audioSampleBuffer;
    float* circularBuffer;
    float currentInputLevel;
    int volatile bufferPos, bufferSize, numSamplesIn;
    CriticalSection bufferAccess;
    
    //Array <float,CriticalSection> circularBuffer; //buffer for the samples
    Array <float> scopeBufferMin;  //should be the same amount of pixels for component size
    Array <float> scopeBufferMax;  //as above
    void calculateScopeBuffers(void);
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Scope)
};

#endif //H_SCOPE