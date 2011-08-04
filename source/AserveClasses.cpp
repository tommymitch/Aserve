/*
*  AserveClasses.cpp
*  jucedemo
*
*  Created by The Butcher on 18/09/2008.
*  Copyright 2008 __MyCompanyName__. All rights reserved.
*
*/

#include "AserveClasses.h"



//==============================================================================
/** Our synth voice oscilator wave..
*/

OsclliatorVoice::OsclliatorVoice()  :	angleDelta (0.0), tailOff (0.0), 
										frequency(0.0), wave(SIN)
{
}

bool OsclliatorVoice::canPlaySound (SynthesiserSound* sound)
{
	return dynamic_cast <OscillatorSound*> (sound) != 0;
}

void OsclliatorVoice::startNote (const int midiNoteNumber, const float velocity,
				SynthesiserSound* sound, const int currentPitchWheelPosition)
{
	currentAngle = 0.0;
	level = velocity;
	tailOff = 0.0;
	
	double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
	double cyclesPerSample = cyclesPerSecond / getSampleRate();
	
	angleDelta = cyclesPerSample * 2.0 * double_Pi;
	
}

void OsclliatorVoice::stopNote (const bool allowTailOff)
{
	if (allowTailOff)
	{
		// start a tail-off by setting this flag. The render callback will pick up on
		// this and do a fade out, calling clearCurrentNote() when it's finished.
		
		if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
							// stopNote method could be called more than once.
			tailOff = 1.0;
	}
	else
	{
		// we're being told to stop playing immediately, so reset everything..
		
		clearCurrentNote();
		angleDelta = 0.0;
	}
}

void OsclliatorVoice::pitchWheelMoved (const int newValue)
{
	// can't be bothered implementing this for the demo!
}

void OsclliatorVoice::controllerMoved (const int controllerNumber, const int newValue)
{
	// not interested in controllers in this case.
}

float OsclliatorVoice::tri(double currentAngle)
{
	/*const float currentSample	= (float) (sin (currentAngle))
	+ (float) (0.11111111111 * sin (3*currentAngle))
	+ (float) (0.04 * sin (5*currentAngle));
	+ (float) (0.02040816327 * sin (7*currentAngle))
	+ (float) (0.01234567901 * sin (9*currentAngle));*/
	const double halfPi = double_Pi/2.0;
	const float currentSample = 
		currentAngle <= halfPi ? (float)(currentAngle*(2.0/(double)double_Pi)):
		currentAngle <= (double_Pi+halfPi)	? (float)(1-(currentAngle-halfPi)*(2.0/double_Pi)) :
		currentAngle <= (double_Pi*2) ? (float)(-1+(currentAngle-halfPi-double_Pi)*(2.0/(double)double_Pi)) : 0.0f;	
	
	return currentSample * 0.8f;
}

float OsclliatorVoice::sqr(double currentAngle)
{
	/*const float currentSample	= (float) (sin (currentAngle))
	+ (float) (0.33333333333 * sin (3*currentAngle))
	+ (float) (0.2 * sin (5*currentAngle));
	+ (float) (0.14285714286 * sin (7*currentAngle))
	+ (float) (0.11111111111 * sin (9*currentAngle));*/
	const float currentSample = currentAngle > double_Pi ? -1.0f:1.0f;
	
	return currentSample * 0.3f;
}

float OsclliatorVoice::saw(double currentAngle)
{
	/*const float currentSample	= (float) (sin (currentAngle))
	+ (float) (0.5 * sin (2*currentAngle))
	+ (float) (0.33333333333 * sin (3*currentAngle))
	+ (float) (0.25 * sin (4*currentAngle))
	+ (float) (0.2 * sin (5*currentAngle));	*/
	const float currentSample = -1.0f + (float)(currentAngle*(2.0/double_Pi));
	
	return currentSample * 0.3f;
}

void OsclliatorVoice::setWave(const int waveform)
{
	wave = waveform;	
}


void OsclliatorVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	if (angleDelta != 0.0)
	{
		if (tailOff > 0)
		{
			while (--numSamples >= 0)
			{
				float currentSample; 
				
				if(wave == TRI)
					currentSample = (float) (tri (currentAngle) * level * tailOff);
				else if(wave == SQR)
					currentSample = (float) (sqr (currentAngle) * level * tailOff);
				else if(wave == SAW)
					currentSample = (float) (saw (currentAngle) * level * tailOff);
				else //SIN
					currentSample = (float) (sin (currentAngle) * level * tailOff);
				
				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					*outputBuffer.getSampleData (i, startSample) += currentSample;
				
				currentAngle += angleDelta;
				const double twoPi = double_Pi*2.0;
				if(currentAngle >= twoPi)
					currentAngle -= twoPi;
				++startSample;
				
				tailOff *= 0.99;
				
				if (tailOff <= 0.005)
				{
					clearCurrentNote();
					
					angleDelta = 0.0;
					break;
				}
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				float currentSample; 
				
				if(wave == TRI)
					currentSample = (float) (tri (currentAngle) * level);
				else if(wave == SQR)
					currentSample = (float) (sqr (currentAngle) * level);
				else if(wave == SAW)
					currentSample = (float) (saw (currentAngle) * level);
				else //SIN
					currentSample = (float) (sin (currentAngle) * level);
				
				for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					*outputBuffer.getSampleData (i, startSample) += currentSample;
				
				currentAngle += angleDelta;
				const double twoPi = double_Pi*2.0;
				if(currentAngle >= twoPi)
					currentAngle -= twoPi;
				++startSample;
			}
		}
	}
}	

//-------------------------------------------
//The synthesiser
//----------------------------

TomSynth::TomSynth()
{
	setNoteStealingEnabled(false);
}
TomSynth::~TomSynth()
{
	
}
//This is a copy of the code from the juce library, modified to include a waveform argument
void TomSynth::noteOnWave (const int wave, const int midiChannel,
						   const int midiNoteNumber,
						   const float velocity)
{
	const ScopedLock sl (lock);
	
	for (int i = sounds.size(); --i >= 0;)
	{
		SynthesiserSound* const sound = sounds.getUnchecked(i);
		
		if (sound->appliesToNote (midiNoteNumber)
			&& sound->appliesToChannel (midiChannel))
		{
			SynthesiserVoice *voice = (findFreeVoice (sound, isNoteStealingEnabled()));
			OsclliatorVoice *oscVoice;
			oscVoice = dynamic_cast<OsclliatorVoice *> (voice);
			oscVoice->setWave(wave);
			startVoice (voice,
						sound, midiChannel, midiNoteNumber, velocity);
		}
	}
}
	
//==============================================================================
/** Our demo synth only has one type of sound, and it's very basic..
*/
OscillatorSound::OscillatorSound (const BitArray& midiNotes_) : midiNotes (midiNotes_)
{
}

bool OscillatorSound::appliesToNote (const int midiNoteNumber)
{
	return midiNotes [midiNoteNumber];
}

//==============================================================================
/** SynthAudioSource implementation
*/
//==============================================================================
SynthAudioSource::SynthAudioSource()
{
	// we'll be mixing two different types of sound, so here we'll create two
	// sets of note maps, putting each sound on a different octave of the keyboard:
	BitArray oscillatorNotes;//, samplerNotes;
	
	int i;
	for (i = 0; i < 128; ++i)
	{
		//if (((i / 12) & 1) != 0)  //only using this oscilator wave at the moment
		oscillatorNotes.setBit (i);
		//else
		// samplerNotes.setBit (i);
	}
	
	// add a wave sound, which will get applied to some of the notes..
	synth.addSound (new OscillatorSound (oscillatorNotes));
	
	// give our synth a few voices that can play the wave sound..
	for (i = 12; --i >= 0;)
		synth.addVoice (new OsclliatorVoice());
	
	//WavAudioFormat wavFormat;
	//AudioFormatReader* audioReader
	//    = wavFormat.createReaderFor (new MemoryInputStream (BinaryData::cello_wav,
	//                                                        BinaryData::cello_wavSize,
	//                                                        false),
	//                                 true);
	
	//synth.addSound (new SamplerSound (T("demo sound"),
	//                                  *audioReader,
	//                                  samplerNotes,
	//                                  74,   // root midi note
	//                                  0.1,  // attack time
	//                                  0.1,  // release time
	//                                  10.0  // maximum sample length
	//                                  ));
	
	//delete audioReader;
	
	// and give the synth some sampler voices to play the sampled sound..
	//for (i = 4; --i >= 0;)
	//synth.addVoice (new SamplerVoice());
}

void SynthAudioSource::prepareToPlay (int samplesPerBlockExpected,
					double sampleRate)
{
	midiCollector.reset (sampleRate);
	
	synth.setCurrentPlaybackSampleRate (sampleRate);
}

void SynthAudioSource::releaseResources()
{
}

void SynthAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	// the synth always adds its output to the audio buffer, so we have to clear it
	// first..
	bufferToFill.clearActiveBufferRegion();
	
	// fill a midi buffer with incoming messages from the midi input.
	MidiBuffer incomingMidi;
	midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
	
	// pass these messages to the keyboard state so that it can update the component
	// to show on-screen which keys are being pressed on the physical midi keyboard.
	// This call will also add midi messages to the buffer which were generated by
	// the mouse-clicking on the on-screen keyboard.
	keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);
	
	// and now get the synth to process the midi events and generate its output.
	synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
}

void SynthAudioSource::noteOnWave(const int wave, const int channel, const int note, const float velocity)
{
	synth.noteOnWave(wave, channel, note, velocity);
}

void SynthAudioSource::noteOff(const int channel, const int note, const bool tailoff)
{
	synth.noteOff(channel, note, tailoff);
}

void SynthAudioSource::allNotesOff()
{
	synth.allNotesOff(1,true);
}

//==============================================================================
OscillatorBankAudioSource::OscillatorBankAudioSource()
{
}

OscillatorBankAudioSource::~OscillatorBankAudioSource()
{
}

//==============================================================================
void OscillatorBankAudioSource::setAmplitude (const int oscillatorNumber, const float newAmplitude)
{
    //sineOscillator.setAmplitude(newAmplitude);
	//squareOscillator.setAmplitude(newAmplitude);
	//sawOscillator.setAmplitude(newAmplitude);
	waveOscillator[oscillatorNumber].setAmplitude(newAmplitude);
}

void OscillatorBankAudioSource::stop (const int oscillatorNumber)
{
    //sineOscillator.stop();
	//squareOscillator.stop();
	//sawOscillator.stop();
	waveOscillator[oscillatorNumber].stop();  
}

void OscillatorBankAudioSource::stopAll (void)
{
    //sineOscillator.stop();
	//squareOscillator.stop();
	//sawOscillator.stop();
	for (int oscillatorNumber = OSCILLATOR_COUNT; --oscillatorNumber >= 0;)
		waveOscillator[oscillatorNumber].stop();
}

void OscillatorBankAudioSource::setFrequency (const int oscillatorNumber, const double newFrequencyHz)
{
    //sineOscillator.setFrequency(newFrequencyHz);
	//squareOscillator.setFrequency(newFrequencyHz);
	//sawOscillator.setFrequency(newFrequencyHz);
	waveOscillator[oscillatorNumber].setFrequency(newFrequencyHz);
}

void OscillatorBankAudioSource::setWave (const int oscillatorNumber, const int wave)
{
	waveOscillator[oscillatorNumber].setWave(wave);
}

//==============================================================================
void OscillatorBankAudioSource::prepareToPlay (int /*samplesPerBlockExpected*/,
                                              double sampleRate_)
{
    //sineOscillator.prepare(sampleRate_);
	//squareOscillator.prepare(sampleRate_);
	//sawOscillator.prepare(sampleRate_);
	for (int oscillatorNumber = OSCILLATOR_COUNT; --oscillatorNumber >= 0;)
		waveOscillator[oscillatorNumber].prepare(sampleRate_);
}

void OscillatorBankAudioSource::releaseResources()
{
}

void OscillatorBankAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
   	
    for (int i = 0; i < info.numSamples; ++i)
    {
        //const float sample = sineOscillator.nextSample();
		//const float sample = squareOscillator.nextSample();
		//const float sample = sawOscillator.nextSample();
		float sample = 0.0f;
		for (int oscillatorNumber = OSCILLATOR_COUNT; --oscillatorNumber >= 0;)
			sample += (float)(waveOscillator[oscillatorNumber].nextSample());
		
        for (int j = info.buffer->getNumChannels(); --j >= 0;)
            *info.buffer->getSampleData (j, info.startSample + i) = sample;
    }
}


