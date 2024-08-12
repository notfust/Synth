/*
  ==============================================================================

    SynthSound.h
    Created: 14 May 2023 3:36:14pm
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote    (int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel)    override { return true; }
    
};
