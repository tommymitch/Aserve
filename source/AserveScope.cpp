/*
 *  AserveScope.cpp
 *  Aserve
 *
 *  Created by tjmitche on 07/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveScope.h"
#include <iostream>


AserveScope::AserveScope()
{
	bufferPos = 0;
	bufferSize = 2048;
	circularBuffer = new float[bufferSize];
	currentInputLevel = 0.0f;
	numSamplesIn = 0;
	
	setOpaque (true);
	startTimer (20);  // repaint every 100 miliseconds
}

AserveScope::~AserveScope()
{
	delete[] circularBuffer;
}

void AserveScope::paint (Graphics& g)
{
	calculateScopeBuffers();
	
	g.fillAll (Colours::lightgrey);
	g.setColour (Colours::darkgrey);
	
	const int top = getHeight();
	
	float middle = (float)top * 0.5f;
	
	g.setColour(Colours::darkgrey);
	g.drawHorizontalLine((int)middle, 0, (float)getWidth());
	
	g.setColour(Colours::white);
	
	if(scopeBufferMin.size() > 0 && scopeBufferMax.size() > 0) // make sure there is some data
	{
		const int scopeBufferSizeMinus1 = scopeBufferMin.size()-1;
		//go through ploting a vertical line from min to max for each pixel
		float oldTop = middle-1+(scopeBufferMax[1]*-middle);
		float oldBottom = middle+1+(scopeBufferMin[1]*-middle);
		for(int i = 1; i < scopeBufferSizeMinus1; i++)
		{
			const float top = middle-1+(scopeBufferMax[i]*-middle);
			const float bottom = middle+1+(scopeBufferMin[i]*-middle);
			
			float lineTop = top > oldBottom ? oldBottom:top;
			float lineBottom = bottom < oldTop ? oldTop:bottom;
			
			g.drawVerticalLine(i, lineTop, lineBottom);	//ofsets look weird because it's been inverted
			
			oldTop=top;
			oldBottom = bottom;
		}
		
		scopeBufferMin.clear();
		scopeBufferMax.clear();
	}
}

void AserveScope::calculateScopeBuffers(void)
{
	const int scopeBufferSize = getWidth();
	const double pixPerSamp = (double)scopeBufferSize / (double)bufferSize;
	
	if(bufferSize > 0 && scopeBufferMin.size() == 0 && scopeBufferMax.size() == 0) //buffers should be zero
	{
		if(pixPerSamp < 1.0)
		{
			double currentPixelAcumulator = 0.0;
			int previousPixel = -1;
			float minVal, maxVal;
			ScopedLock sl(bufferAccess);
			for(int sampleNum = 0; sampleNum < bufferSize; sampleNum++, currentPixelAcumulator += pixPerSamp)
			{
				const int currentPixel = (int)currentPixelAcumulator; //rounds down to pixel
				
				if(currentPixel > scopeBufferSize)//debug
					std::cout << "too many pixels" << std::endl;
				
				int readValue = bufferPos - sampleNum;
				if(readValue < 0)
					readValue += bufferSize;
				
				float value = circularBuffer[readValue];
				//std::cout << " value " << value << " Readval " << readValue  << " sampleNum " << sampleNum <<std::endl;
				
				if(currentPixel != previousPixel) //first sample of the next pixel
				{
					
					if(previousPixel >= 0) //not the first time (it's -1)
					{
						scopeBufferMin.add(minVal); //store last pixels min
						scopeBufferMax.add(maxVal); //and max
					}
					
					minVal = maxVal = value;	//reset min&max to the first sample of this pixel
					previousPixel = currentPixel;//increment the draw index
				}
				else if(sampleNum == (bufferSize-1))//this gets the last one
				{
					scopeBufferMin.add(minVal); //store last pixels min
					scopeBufferMax.add(maxVal); //and max
				}
				else	//still checking sample for the same pixel
				{
					if(value < minVal)		//store the min
						minVal = value;
					if(value > maxVal)		//and the max
						maxVal = value;
				}
			}
		}
		else
		{
			std::cout << "scope size is bigger that sample buffer size, this  just shouldn't be" << std::endl;
		}
	//scope is 1000 pixes buffer 2000 samples 
	//find out how many samples per pixel samp/pix
	//2000 / 1000 = 2 samples per pixel
	//find out pixel per sample = pix/samp
	//1000 / 2000 = 0.5 pixels per sample
	
	//loop for each sample
	//cast pix/sampe acumulator to int
	//if the value is the same as it was last sample check for max min
	//if not (we've progressed to next pixel) store max/min vals in scope array, reset max/min, increment last sample
	}	
	
	
}

void AserveScope::resized()
{
	
}

void AserveScope::timerCallback()
{
	repaint();
}

void AserveScope::addSample (const float sample)
{
	currentInputLevel += fabsf (sample);
	
	const int samplesToAverage = 128;
	
	if (++numSamplesIn > samplesToAverage)
	{
		circularBuffer [bufferPos++ % bufferSize] = currentInputLevel / samplesToAverage;
		
		numSamplesIn = 0;
		currentInputLevel = 0.0f;
	}
}

void AserveScope::audioDeviceIOCallback (const float** inputChannelData,
													   int totalNumInputChannels,
													   float** outputChannelData,
													   int totalNumOutputChannels,
													   int numSamples)
{
	ScopedLock sl(bufferAccess);
	for (int j = 0; j < numSamples; ++j)
	{
		float inMax = 0;
		for (int i = 0; i < totalNumInputChannels; ++i)
		{
			if(fabs(inputChannelData [i][j]) > inMax) 
				inMax = inputChannelData [i][j];	//max of all channels
		}
		circularBuffer [bufferPos] = inMax;
		if(++bufferPos == bufferSize) 
			bufferPos = 0;
	}
}

void AserveScope::audioDeviceAboutToStart (AudioIODevice*)
{
	zeromem (circularBuffer, sizeof (float) * bufferSize);
}

void  AserveScope::audioDeviceStopped()
{
	zeromem (circularBuffer, sizeof (float) * bufferSize);
}


