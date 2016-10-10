/*
 *  aservelib.h
 *  aserve communication functions
 *  Created by Tom Mitchell 2009
 *
 */

#ifndef __ASERVELIB_H__
#define __ASERVELIB_H__

/**
 Function to control aserve's oscillators. 
 
 Arguments control the oscillator parameters. Setting the amplitude
 of an oscillator to 0.0 will switch it off.
 
 @param		oscillatorNumber	selects the oscillator: 0 - 15
 @param		frequency			sets the selected oscillator's frequency: 0.0Hz - 20000.0Hz
 @param		amplitude			sets the selected oscillator's amplitude: 0.0 to 1.0
 @param		wave				sets the selected oscillator's waveform: 0 = sine, 1 = pulse, 2 = saw
 
 Example usage:
 @code
 //Set the first oscillator to a 1kHz sinusoidal tone at max amplitude  
 aserveOscillator(0, 1000, 1.0, 0); 
 @endcode
 */
void aserveOscillator (int oscillatorNumber, float frequency, float amplitude, int wave);

/**
 Returns the next note message received on aserve's active MIDI ports. 
 
 Execution is paused until the next MIDI note message is received, whereupon 
 this function returns the received note number, in range 0 - 127. 
 
 @return The number of the next note received number.
 
 Example usage:
 @code
 //assign the received MIDI note to the integer variable note  
 note = aserveGetNote();  
 @endcode
 */
int aserveGetNote();

/**
 Returns the velocity of the last note message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 127, where a value of 0 indicates that the last note was a note off. 
 If a note message has not been received since the program was launched, a value of 0 is returned. 
 
 @return The velocity value of the last note received.
 
 Example usage:
 @code
 //assign note velocity to the integer variable velocity  
 velocity = aserveGetVelocity();  
 @endcode
 */
int aserveGetVelocity();

/**
 Returns the channel of the last note message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 15, correlating to MIDI channels 1 - 16. If a note message 
 has not been received since the program was launched, a value of 0 is returned. 
 
 @return The channel of the last note recieved. 
 
 Example usage:
 @code
 //assign note channel to the integer variable channel  
 channel = aserveGetNoteChannel();  
 @endcode
 */
int aserveGetNoteChannel();

/**
 Returns continuous controller data received on aserve's active MIDI ports. 
 
 Returned integer (range 0 - 127) is the last value received on the specified 
 controller number (also 0 - 127). If a controller message has not been received on the specified 
 controller number since the program was launched, a value of 0 is returned. 
 
 @param		number		specifies the controller number: 0 - 127
 
 @return The control value of the specified control number. 
 
 Example usage:
 @code
 //assign the modwheel continuous controller value to the integer value
 value = aserveGetControl(1); 
 @endcode
 */
int aserveGetControl (int number);

/**
 Returns the channel of the last continuous controller data received on aserve's active MIDI ports. 
 
 Returned integer has the range 0 - 15. If a controller message has not been received on the specified 
 controller number since the program was launched, a value of 0 is returned.
 
 @param		number	Select which controller number: 0 - 127
 
 @return The control channel. 
 
 Example usage:
 @code
 //assigns channel on which the last modwheel message was received to the integer channel
 channel = aserveGetControlChannel(1); 
 @endcode
 */
int aserveGetControlChannel (int number);

/**
 Returns the number of the last program change message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 127. If a program change message has not been received since the 
 program was launched, a value of 0 is returned. 
 
 @return The program number.
 
 Example usage:
 @code
 //assigns program number to the integer variable program
 program = aserveGetProgram(); 
 @endcode
 */
int aserveGetProgram();

/**
 Returns the channel of the last program change message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 15. If a note message has not been received since the program was 
 launched, a value of 0 is returned. 
 
 @return The program channel.
 
 Example usage:
 @code
 //assigns program number to the integer variable program
 program = aserveGetProgram(); 
 @endcode
 */
int aserveGetProgramChannel();

/**
 Returns the value of the last channel after touch message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 127. If an after touch message has not been received since the program 
 was launched, a value of 0 is returned. 
 
 @return The aftertouch value.
 
 Example usage:
 @code
 //assigns after touch value to the integer variable afterTouch 
 afterTouch = aserveGetAfterTouch();  
 @endcode
 */
int aserveGetAfterTouch();

/**
 Returns the channel number of the last channel after touch message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 15. If a note message has not been received since the program was launched, 
 a value of 0 is returned. 
 
 @return The aftertouch channel.
 
 Example usage:
 @code
 //assigns after touch channel to the integer variable channel
 channel = aserveGetAfterTouchChannel(); 
 @endcode
 */
int aserveGetAfterTouchChannel();

/**
 Returns the value of the last pitchbend message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 16383. If a pitchbend message has not been received since the 
 program was launched, a value of 0 is returned. 
 
 @return The pitchbend value.
 
 Example usage:
 @code
 //assigns pitch bend value to the integer bend 
 bend = aserveGetPitchBend(); 
 @endcode
 */
int aserveGetPitchBend();

/**
 Returns the channel number of the last pitchbend message received on aserve's active MIDI ports. 
 
 Returned value range is 0 - 15. If a pitchbend message has not been received since the program 
 was launched, a value of 0 is returned. 
 
 @return The pitchbend channel.
 
 Example usage:
 @code
 //assigns pitch bend channel to the integer variable channel 
 channel = aserveGetPitchBendChannel(); 
 @endcode
 */
int aserveGetPitchBendChannel();

/**
 Causes program execution to pause for the specified time in milliseconds. 
 
 @param		milliseconds		sleep time in ms
 
 Example usage:
 @code
 //pause for 1 second
 aserveSleep(1000); 
 @endcode
 */
void aserveSleep (int milliseconds);

/**
 Returns the number of milliseconds elapsed since program startup 
 
 @return The time in ms.
 
 Example usage:
 @code
 //assigns the time since program startup in ms to time
 time = aserveGetTime(); 
 @endcode
 */
float aserveGetTime();

/**
 Returns the state of aserve’s sequencer window. 
 
 Each row of the sequencer sub window in aserve represents a 16-bit integer. This function returns
 the current value represented by the bit pattern on the specified row. The range of the returned value 
 is 0 - 65535.
 
 @param		row		specifies the row to return: 0 - 7
 
 @return The value at the specified row.
 
 Example usage:
 @code
 //assigns the number represented on the top row to the integer value 
 value = aserveGetSequenceRow(0); 
 @endcode
 */
int aserveGetSequenceRow (int row);

/**
 Sets the state of aserve's sequencer sub window.
 
 Each row of the sequencer sub window in aserve represents a 16-bit integer. This function sets
 the specified row to the specified bit pattern.
 
 @param		row		specifies the row to set: 0 - 7
 @param		value	specifies the bit pattern to display: 0 - 65535
 
 Example usage:
 @code
 //switch all buttons on the bottom row to on (1). 
 aserveSetSequenceRow(7, 65535); 
 @endcode
 */
void aserveSetSequenceRow (int row, int value);

/**
 Loads a sample in to the sample window.
 
 The sample window can load up to 8 .wav/.aiff files located on the file system. This function allows 
 samples to be loaded programmatically. 
 
 @param		row		specifies the row into which the file will be loaded: 0 - 7
 @param		path	specifies the path to the file to be loaded (in windows use '/' and not '\' in the file path)
 
 Example usage:
 @code
 //load the sample at the path "/Users/tjmitche/bd.wav" into the top row of the sample window. 
 aserveLoadSample(0, "/Users/tjmitche/bd.wav");
 @endcode
 
 @see aserveSample
 */
void aserveLoadSample (int row, const char *path);

/**
 Play back files in the sample window.
 
 File loaded at specified row is played back at the specified amplitude. An amplitude value of 
 0.0 will stop playback. 
 
 @param		row			specifies the sample row to playback: 0 - 7
 @param		amplitude	specifies the playback amplitude: 0.0 - 1.0
 
 Example usage:
 @code
 //plays the sample on the bottom row at max amplitude
 aserveSample(7, 1.0); 
 @endcode
 
 @see aserveLoadSample
 */
void aserveSample (int row, float amplitude);

/**
 Transmits the specified MIDI message on aserve's active output ports.
 
 @param		statusbyte	specifies the status byte of the message to be transmitted: 128 - 255
 @param		databyte1	specifies the first data byte of the message to be transmitted: 0 - 127
 @param		databyte2	specifies the second data byte of the message to be transmitted: 0 - 127
 
 Example usage:
 @code
 // send a note on message on channel 1 for note C4 at max velocity 
 aserveSetMidi(144, 60, 127);
 @endcode
 
 */
void aserveSetMidi (const int statusbyte, const int databyte1, const int databyte2);


/**
 Sets a callback function to be called whenever a MIDI message is received.
 
 By calling this function with an argument that is a pointer to a function declared with the correct 
 prototype, the function that is pointed to will be called whenever a MIDI messages is received 
 and the three integer arguments will have the values of the three bytes of the corresponding 
 MIDI message. 
 
 @param		f	Your callback function.
 
 Example usage:
 @code
 //set MIDI callback function
 aserveSetMidiCallback(midiCallback); 
 
 //declaration for midiCallBack (in your program)
 void midiCallback(int statusbyte, int databyte1, int databyte2)
 
 //example definition for midiCallBack (also in your program)
 void midiCallback(int statusbyte, int databyte1, int databyte2) 
 { 
 printf("statusbyte => %d\n", statusbyte); 
 printf("databyte1 => %d\n", databyte1); 
 printf("databyte2 => %d\n", databyte2); 
 }  
 @endcode
 
 The above example will cause your function midiCallback() to be called whenever a midi 
 message is received on aserve's active input ports. In this instance the values of the 
 corresponding status and data bytes are printed. 
 */
void aserveSetMidiCallback (void (*f)(int, int, int));

#ifndef WIN32 
/**
 Synthesises text into speech (Mac only).
 
 @param		message	the text to be synthesised.
 
 Example usage:
 @code
 //synthesises the text
 aserveSay("hello world"); 
 @endcode
 */
void aserveSay (const char *message);
#endif//JUCE_MAC 
#endif //__ASERVELIB_H__


