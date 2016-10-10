/*
 *  OscillatorManager.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_OSCILLATORMANAGER
#define H_OSCILLATORMANAGER

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AserveOscillators.h"

/**
 Manages Aserve's oscillators
 */

class OscillatorManager : public AudioSource
{
public:
    enum OscillatorManagerConstants 
    {
        NumOscillators = 16,
        NumWaveforms = WaveOscillator::NumWaveforms
    };
    /** 
     Constructor 
     */
    OscillatorManager();
    
    /** 
     Destructor 
     */
    ~OscillatorManager();

    /** 
     Sets the signal's amplitude. 
     */
    void setAmplitude (const int oscillatorNumber, const float newAmplitude);
    
    /** 
     Sets the signal's frequency. 
     */
    void setFrequency (const int oscillatorNumber, const double newFrequencyHz);
    
    /**
     Set waveform of the specified oscillator
     */
    void setWaveform (const int oscillatorNumber, const int wave);
    
    /** 
     Stops all oscillators in back with a tailoff 
     */
    void stopAll(void);
	
    /** 
     Stops the specified oscillator with a tailoff 
     */
    void stop(const int oscillatorNumber);
	
    /** Sets the signal's waveform. */
    void setWave (const int oscillatorNumber, const int wave);
    //AudioSouce Callbacks
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    virtual void releaseResources();
    virtual void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
private:
    OwnedArray<WaveOscillator> oscillators;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorManager)
};  


#endif //H_OSCILLATORMANAGER