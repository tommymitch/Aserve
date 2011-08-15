/*
 *  OscillatorManager.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscillatorManager.h"

OscillatorManager::OscillatorManager()
{
    for (int i = 0; i < NumOscillators; i++) 
    {
        oscillators.add(new WaveOscillator());
    }
    
}

OscillatorManager::~OscillatorManager()
{
}

//==============================================================================
void OscillatorManager::setAmplitude (const int oscillatorIndex, const float newAmplitude)
{
    if (isPositiveAndBelow(oscillatorIndex, static_cast<int>(NumOscillators))) 
        oscillators[oscillatorIndex]->setAmplitude(newAmplitude);
}

void OscillatorManager::stop (const int oscillatorIndex)
{
    if (isPositiveAndBelow(oscillatorIndex, static_cast<int>(NumOscillators)))
        oscillators[oscillatorIndex]->stop();  
}

void OscillatorManager::stopAll (void)
{
	for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;)
		oscillators[oscillatorIndex]->stop();
}

void OscillatorManager::setFrequency (const int oscillatorIndex, const double newFrequencyHz)
{
    if (isPositiveAndBelow(oscillatorIndex, static_cast<int>(NumOscillators))) 
        oscillators[oscillatorIndex]->setFrequency(newFrequencyHz);
}

void OscillatorManager::setWave (const int oscillatorIndex, const int wave)
{
    if (isPositiveAndBelow(oscillatorIndex, static_cast<int>(NumOscillators))) 
        oscillators[oscillatorIndex]->setWave(wave);
}

//==============================================================================
void OscillatorManager::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate_)
{
	for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;)
		oscillators[oscillatorIndex]->prepare(sampleRate_);
}

void OscillatorManager::releaseResources()
{
    
}

void OscillatorManager::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    for (int i = 0; i < info.numSamples; ++i)
    {
		float sample = 0.0f;
		for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;)
			sample += (float)(oscillators[oscillatorIndex]->nextSample());
		
        for (int j = info.buffer->getNumChannels(); --j >= 0;)
            *info.buffer->getSampleData (j, info.startSample + i) = sample;
    }
}
