/*
 *  AserveOscillators.cpp
 *  jucedemo
 *
 *  Created by Thomas Mitchell on 25/09/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveOscillators.h"
#include "../JuceLibraryCode/JuceHeader.h" // this is just for the juce PI and maths stuff

#include <limits>

//-------------------------------------------
// Oscillator base class
Oscillator::Oscillator() 
:		frequency (1000.0),
        amplitude (0.0f),
		sampleRate (44100.0),
		tailOff(0.0)
{
	
}
Oscillator::~Oscillator()
{
	
}

void Oscillator::prepare(double sampleRate_)
{
    sampleRate = sampleRate_;
	this->reset();
}

void Oscillator :: reset()
{
	amplitude = 0.0;		//must reset amplitude to prevent rendering when == 0
}

void Oscillator::setFrequency(const double val)
{
	frequency = val;
}

void Oscillator::setAmplitude(const float val)
{
	amplitude = val;
	tailOff = 0.0;
}

//use this to fade out setAmplitude(0) for instintanious holdup!
void Oscillator::stop(void)
{
	if (tailOff == 0.0)//only need to begin a tailoff if it has not already begun
		tailOff = 1.0;
}


double Oscillator::nextSample(void)
{
	double sample = 0.0;
	
	
	if(amplitude > 0.0)//don't calculate anything unless necessary
	{	
		
		
		if (tailOff > 0)			//if fading out incorporate the fade
		{
			sample = amplitude * (float) renderWaveShape() * tailOff;
			tailOff *= 0.99;
			
			if (tailOff <= 0.005)
			{
				this->reset();
			}
		}
		else
		{
			sample = amplitude * (float) renderWaveShape();
		}
	}
	return sample;
}

//-------------------------------------------
//Sinusoidal Oscilator - Not Necessary 
SineOscillator::SineOscillator() 
: 
currentAngle (0.0),
angleDelta (0.0)
{
	
}
SineOscillator::~SineOscillator()
{
	
}



void SineOscillator :: reset()
{
	amplitude = 0.0;
	angleDelta = 0.0;
}

void SineOscillator::setFrequency(const double val)
{
	frequency = val; 
	angleDelta = 0;
}




double SineOscillator::renderWaveShape(void)
{
	const double sample = sin(currentAngle);
	
	if (angleDelta == 0.0)		//if the frequency ha been changed reset the stepsize
		angleDelta = double_Pi * 2.0 / (sampleRate / frequency);
	
	currentAngle += angleDelta;
	const double twoPi = double_Pi*2.0;
	if(currentAngle >= twoPi)
		currentAngle -= twoPi;
	
	return sample;
}

//-------------------------------------------
//Square Oscilator 
//Adapted from The Synthesis ToolKit in C++ (STK) 
//By Perry R. Cook and Gary P. Scavone, 1995-2007.

SquareOscillator::SquareOscillator() 
: nHarmonics_(0),
lastOutput_(0)
{
	this->setFrequency( frequency );
	this->reset();
}
SquareOscillator::~SquareOscillator()
{
	
}

void SquareOscillator :: reset()
{
	amplitude = 0.0;
	phase_ = 0.0;
	lastOutput_ = 0;
	dcbState_ = 0.0;
	lastBlitOutput_ = 0;
}

void SquareOscillator::prepare(double sampleRate_)
{	
    sampleRate = sampleRate_;
	this->reset();
	this->updateHarmonics();
}

void SquareOscillator::setFrequency(const double val)
{
	frequency = val;
	// By using an even value of the parameter M, we get a bipolar blit
	// waveform at half the blit frequency.  Thus, we need to scale the
	// frequency value here by 0.5. (GPS, 2006).
	p_ = 0.5 * sampleRate / frequency;
	rate_ = double_Pi / p_;
	this->updateHarmonics();
}

void SquareOscillator::updateHarmonics( void )
{
	// Make sure we end up with an even value of the parameter M here.
	if ( nHarmonics_ <= 0 ) 
	{
		unsigned int maxHarmonics = (unsigned int) floor( 0.5 * p_ );
		m_ = 2 * (maxHarmonics + 1);
	}
	else
		m_ = 2 * (nHarmonics_ + 1);
	
	a_ = m_ / p_;
}

double SquareOscillator :: renderWaveShape( void )
{
	double temp = lastBlitOutput_;
	const double twoPi = double_Pi*2.0;
	// A fully  optimized version of this would replace the two sin calls
	// with a pair of fast sin oscillators, for which stable fast 
	// two-multiply algorithms are well known. In the spirit of STK,
	// which favors clarity over performance, the optimization has 
	// not been made here.
	
	// Avoid a divide by zero, or use of a denomralized divisor
	// at the sinc peak, which has a limiting value of 1.0.
	double denominator = sin( phase_ );
	if ( fabs( denominator )  < std::numeric_limits<double>::epsilon() ) {
		// Inexact comparison safely distinguishes betwen *close to zero*, and *close to PI*.
		if ( phase_ < 0.1f || phase_ > twoPi - 0.1f )
			lastBlitOutput_ = a_;
		else
			lastBlitOutput_ = -a_;
	}
	else {
		lastBlitOutput_ =  sin( m_ * phase_ );
		lastBlitOutput_ /= p_ * denominator;
	}
	
	lastBlitOutput_ += temp;
	
	// Now apply DC blocker.
	lastOutput_ = lastBlitOutput_ - dcbState_ + 0.999 * lastOutput_;
	dcbState_ = lastBlitOutput_;
	
	phase_ += rate_;
	if ( phase_ >= twoPi ) phase_ -= twoPi;
	
	return lastOutput_;
}

//-------------------------------------------
//Saw Oscilator 
//Adapted from The Synthesis ToolKit in C++ (STK) 
//By Perry R. Cook and Gary P. Scavone, 1995-2007.

SawOscillator::SawOscillator() 
: nHarmonics_(0),
lastOutput_(0)
{
	this->setFrequency( frequency );
	this->reset();
}
SawOscillator::~SawOscillator()
{
	
}

void SawOscillator :: reset()
{
	amplitude = 0.0;
	phase_ = 0.0f;
	state_ = 0.0;
	lastOutput_ = 0;
}

void SawOscillator::prepare(double sampleRate_)
{	
    sampleRate = sampleRate_;
	this->reset();
	this->updateHarmonics();
}

void SawOscillator::setFrequency(const double val)
{
	frequency = val;
	// By using an even value of the parameter M, we get a bipolar blit
	// waveform at half the blit frequency.  Thus, we need to scale the
	// frequency value here by 0.5. (GPS, 2006).
	
	this->updateHarmonics();
	
	p_ = sampleRate / frequency;
	C2_ = 1 / p_;
	rate_ = double_Pi * C2_;
	this->updateHarmonics();
}

void SawOscillator::updateHarmonics( void )
{
	if ( nHarmonics_ <= 0 ) 
	{
		unsigned int maxHarmonics = (unsigned int) floor( 0.5 * p_ );
		m_ = 2 * maxHarmonics + 1;
	}
	else
		m_ = 2 * nHarmonics_ + 1;
	
	a_ = m_ / p_;
}

double SawOscillator :: renderWaveShape( void )
{
	// The code below implements the BLIT algorithm of Stilson and
	// Smith, followed by a summation and filtering operation to produce
	// a sawtooth waveform.  After experimenting with various approaches
	// to calculate the average value of the BLIT over one period, I
	// found that an estimate of C2_ = 1.0 / period (in samples) worked
	// most consistently.  A "leaky integrator" is then applied to the
	// difference of the BLIT output and C2_. (GPS - 1 October 2005)
	
	// A fully  optimized version of this code would replace the two sin 
	// calls with a pair of fast sin oscillators, for which stable fast 
	// two-multiply algorithms are well known. In the spirit of STK,
	// which favors clarity over performance, the optimization has 
	// not been made here.
	
	// Avoid a divide by zero, or use of a denormalized divisor 
	// at the sinc peak, which has a limiting value of m_ / p_.
	
	double denominator = sin( phase_ );
	
	if ( fabs(denominator) <= std::numeric_limits<double>::epsilon() )
		lastOutput_ = a_;
	else 
	{
		lastOutput_ =  sin( m_ * phase_ );
		lastOutput_ /= p_ * denominator;
	}
	
	lastOutput_ += state_ - C2_;
	state_ = lastOutput_ * 0.995;
	
	phase_ += rate_;
	if ( phase_ >= double_Pi ) phase_ -= double_Pi;
    
	return lastOutput_;
}

//------------------------------------------------
//an oscillator class that does it all
//If I were to improvr this I would probaly implement all the rendering 
//within this class such that the frequency, amp, tailoff, etc variables were shared.

WaveOscillator::WaveOscillator() : wave(0)
{
	
}
	
WaveOscillator::~WaveOscillator()
{
	
}

void WaveOscillator::setWave(int val)
{
	wave = val;
}

void WaveOscillator::setFrequency(const double val)
{
	sinOscillator.setFrequency(val);
	squareOscillator.setFrequency(val);
	sawOscillator.setFrequency(val);
}

void WaveOscillator::setAmplitude(const float val)
{
	sinOscillator.setAmplitude(val);
	squareOscillator.setAmplitude(val);
	sawOscillator.setAmplitude(val);
}

void WaveOscillator::prepare(double sampleRate_)
{
	sinOscillator.prepare(sampleRate_);
	squareOscillator.prepare(sampleRate_);
	sawOscillator.prepare(sampleRate_);
}

void WaveOscillator::stop(void)
{
	sinOscillator.stop();
	squareOscillator.stop();
	sawOscillator.stop();
}

double WaveOscillator::nextSample(void)
{
	double sample = 0;
	switch(wave)
	{
		case 1 : sample = squareOscillator.nextSample(); break; 
		case 2 : sample = sawOscillator.nextSample(); break; 
		default: sample = sinOscillator.nextSample(); break;
	}
	return sample;
}


