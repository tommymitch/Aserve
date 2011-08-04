/*
 *  AserveClasses.h
 *  jucedemo
 *
 *  Created by The Butcher on 18/09/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __ASERVE_CLASSES__
#define __ASERVE_CLASSES__

#include <juce.h>
#include "AserveOscillators.h"

//==============================================================================
/** Our synth only has one type of sound, and it's very basic..
*/
class OscillatorSound : public SynthesiserSound
{
public:
    OscillatorSound (const BitArray& midiNotes_);
    bool appliesToNote (const int midiNoteNumber);
    bool appliesToChannel (const int midiChannel) { return true; };
	
private:
    // this will contain the notes that this sound is attached to.
    BitArray midiNotes;
};

//==============================================================================
/** Our synth voice oscilator wave..
*/

enum
{
	SIN,
	TRI,
	SQR,
	SAW	
};

class OsclliatorVoice : public SynthesiserVoice
{
public:
    OsclliatorVoice();
    bool canPlaySound (SynthesiserSound* sound);
    void startNote (const int midiNoteNumber, const float velocity,
                    SynthesiserSound* sound, const int currentPitchWheelPosition);
    void stopNote (const bool allowTailOff);
    void pitchWheelMoved (const int newValue);
    void controllerMoved (const int controllerNumber, const int newValue);
	float tri(double currentAngle);
	float sqr(double currentAngle);
	float saw(double currentAngle);
	void setWave(const int waveform);
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples);
	
private:
    double currentAngle, angleDelta, level, tailOff, frequency;
	int wave;
};


//==============================================================================
/** The synthesiser in the heart of the app.
*/
class TomSynth : public Synthesiser
{
public:
	TomSynth();
	~TomSynth();
	//This is a copy of the code from the juce library, modified to include a waveform argument
	void noteOnWave (const int wave, const int midiChannel,
					 const int midiNoteNumber,
					 const float velocity);
};

//==============================================================================
/** This is an audio source that streams the output of our demo synth.
*/
class SynthAudioSource  : public AudioSource
{
public:
    //==============================================================================
    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    MidiMessageCollector midiCollector;
	
    // this represents the state of which keys on our on-screen keyboard are held
    // down. When the mouse is clicked on the keyboard component, this object also
    // generates midi messages for this, which we can pass on to our synth.
    MidiKeyboardState keyboardState;
	
    // the synth itself!
    TomSynth synth;
	
    //==============================================================================
    SynthAudioSource();
	
    void prepareToPlay (int samplesPerBlockExpected,
                        double sampleRate);
    	
	void releaseResources();
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
	void noteOnWave(const int wave, const int channel, const int note, const float velocity);
	void noteOff(const int channel, const int note, const bool tailoff);
	void allNotesOff();
};

//This is set in the message thread so there should be some form of lock when setting freq
//and amp, and then again when rendering!
class OscillatorBankAudioSource  : public AudioSource
{
	public:
		//==============================================================================
		/** Creates a ToneGeneratorAudioSource. */
		OscillatorBankAudioSource();
		
		/** Destructor. */
		~OscillatorBankAudioSource();
		
		//==============================================================================
		/** Sets the signal's amplitude. */
		void setAmplitude (const int oscillatorNumber, const float newAmplitude);
		
		/** Sets the signal's frequency. */
		void setFrequency (const int oscillatorNumber, const double newFrequencyHz);
		
		/** Stops all oscillators in back with a tailoff */
		void stopAll(void);
	
		/** Stops the specified oscillator with a tailoff */
		void stop(const int oscillatorNumber);
	
		/** Sets the signal's waveform. */
		void setWave (const int oscillatorNumber, const int wave);
	
		//==============================================================================
		/** Implementation of the AudioSource method. */
		void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
		
		/** Implementation of the AudioSource method. */
		void releaseResources();
		
		/** Implementation of the AudioSource method. */
		void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
		
		
		//==============================================================================
		
	private:
		//==============================================================================
#define OSCILLATOR_COUNT 16
		WaveOscillator waveOscillator[OSCILLATOR_COUNT];
};


#endif   // __ASERVE_CLASSES__