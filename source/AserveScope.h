/*
 *  AserveScope.h
 *  Aserve
 *
 *  Created by tjmitche on 07/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef __ASERVESCOPE__
#define __ASERVESCOPE__

#include <juce.h>

class AserveScope   :	public Component,
						public Timer,
						public AudioIODeviceCallback
{
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
	
public:	
	AserveScope();
	~AserveScope();
	void paint (Graphics& g);
    void resized();
	void addSample (const float sample);
	void timerCallback();
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);
	/**
	 This fundtion takes audio data and is called synchronosly from the audio thread
	 and adds it to the audioSampleBuffer when the buffer is full it is coppied to a 
	 display buffer which is then processed and repainted asyncronously, the buffer is 
	 then overwritten.
	 */
	void audioDeviceAboutToStart (AudioIODevice*);
	void audioDeviceStopped();
	void processAudio();
	void changeListenerCallback (void *objectThatHasChanged);
	/**
	 Converts the audio data to vertical lines in an array that contains minMax data for exah
	 display pixel
	 */
	
	
};



#endif   // __ASERVESCOPE__