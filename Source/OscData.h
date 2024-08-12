/*
  ==============================================================================

    OscData.h
    Created: 14 May 2023 3:46:57pm
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OscUI.h"

using namespace juce;

class OscData
    : public dsp::Oscillator<float>
{
public:
    void  prepareToPlay (double sampleRate,
                         int    samplesPerBlock,
                         int    outputChannels);
    void  setType (const int oscSelection);
    void  setGain (const float levelInDecibels);
    void  setOscPitch (const int pitch);
    void  setFreq (const int midiNoteNumber);
    void  setFmOsc (const float freq,
                    const float depth);
    void  renderNextBlock (dsp::AudioBlock<float>& audioBlock);
    float processNextSample (float input);
    void  setParams (const int   oscChoice,
                     const float oscGain,
                     const int   oscPitch,
                     const float fmFreq,
                     const float fmDepth);
    void  resetAll();

private:
    dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); }};
    dsp::Gain<float> gain;
    int   lastPitch    { 0 };
    int   lastMidiNote { 0 };
    float fmDepth      { 0.0f };
    float fmModulator  { 0.0f };
};
