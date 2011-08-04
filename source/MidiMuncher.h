/*
 *  MidiMuncher.h
 *  Aserve
 *
 *  Created by tjmitche on 05/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MIDIMUNCHER__
#define __MIDIMUNCHER__

#include <juce.h>

class MidiMuncher2   :  public MidiInputCallback,
						public ActionBroadcaster
	{
	private:
	
		
	public:	
		MidiMuncher2();
		~MidiMuncher2();
		void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message);
	};


#endif   // __MIDIMUNCHER__