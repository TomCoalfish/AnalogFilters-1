// ECS7012P Music and Audio Programming
// School of Electronic Engineering and Computer Science
// Queen Mary University of London
// Spring 2020

// ADSR.cpp: implement the ADSR (attack-decay-sustain-release) class

#include "ADSR.h"

// Constructor. Set up some default parameters.
// We can also use initialisation lists before the 
// start of the curly braces to set these values
ADSR::ADSR()
{
	attackTime = 0.001;
	decayTime = 0.001;
	sustainLevel = 1;
	releaseTime = 0.001;

	state = kADSRStateOff;
	level = 0.0;
	increment = 0.0;
	sampleRate = 44100.0;
}

// Set the sample rate, used for all calculations
void ADSR::setSampleRate(float rate) {
	sampleRate = rate;
}

// Start the envelope, going to the Attack state
void ADSR::trigger() {
	// Go to the Attack state from whichever state we were in
	state = kADSRStateAttack;
    increment = (1.0 - level) / (attackTime * sampleRate);
}

// Stop the envelope, going to the Release state
void ADSR::release() {
	// Go to the Release state from whichever state we were in
	state = kADSRStateRelease;
	increment = (0.0 - level) / (releaseTime * sampleRate);
}

// Calculate the next sample of output, changing the envelope
// state as needed
float ADSR::getNextValue() {
	
	// Look at the state we're in to decide what value to return. 
	// This function handles the outputs within the state but
	// does not handle the transitions caused by external note events.
	// Those are done in trigger() and release().
    if(state == kADSRStateOff) {
		level = 0;
	}
	else if(state == kADSRStateAttack) {
		// Increment envelope until it reaches 1.0
		level += increment;
		if(level >= 1.0) {
			state = kADSRStateDecay;
			increment = (sustainLevel - 1.0) / (decayTime * sampleRate);
		}
	}
	else if(state == kADSRStateDecay) {
		// Decrement envelope until it reaches the sustain level
		level += increment;
		if(level <= sustainLevel) {
			state = kADSRStateSustain;
			increment = 0;
		}
	}
	else if(state == kADSRStateSustain) {
		// Do nothing here because release() will take us
		// to the next state
	}
	else if(state == kADSRStateRelease) {
		// Wait until the envelope returns to 0
		level += increment;
		if(level <= 0.0) {
			level = 0.0;		// Make sure we clip the envelope to 0
			state = kADSRStateOff;
		}
	}
    	
    // Return the current output level
    return level;
}

// Indicate whether the envelope is active or not (i.e. in
// anything other than the Off state
bool ADSR::isActive() {
	return (state != kADSRStateOff);
}

// Destructor
ADSR::~ADSR() {
	// Nothing to do here
}