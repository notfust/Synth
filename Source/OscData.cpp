/*
  ==============================================================================

    OscData.cpp
    Created: 14 May 2023 3:46:57pm
    Author:  test

  ==============================================================================
*/

#include "OscData.h"


using namespace juce;

void OscData::prepareToPlay (double sampleRate,
                             int    samplesPerBlock,
                             int    outputChannels)
{
    resetAll();
    
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    prepare (spec);
    fmOsc.prepare (spec);
    gain.prepare (spec);
}

void OscData::setType (const int oscSelection)
{
    switch (oscSelection)
    {
        // Sine
        case 0:
            initialise ([](float x) {
                return std::sin (x);
            });
            break;

        // Saw
        case 1:
            initialise ([] (float x) {
                return x / MathConstants<float>::pi;
            });
            break;

        // Square
        case 2:
            initialise ([] (float x) {
                return x < 0.0f ? -1.0f : 1.0f;
            });
            break;
            
        // Triangle
        case 3:
            initialise ([] (float x) {
                return x < 0.0f ? -1.0f : 1.0f;
            });
            break;

        default:
            jassertfalse;
            break;
    }
}

void OscData::setGain (const float levelInDecibels)
{
    gain.setGainDecibels (levelInDecibels);
}

void OscData::setOscPitch (const int pitch)
{
    lastPitch = pitch;
    setFrequency (MidiMessage::getMidiNoteInHertz ((lastMidiNote + lastPitch) + fmModulator));

}

void OscData::setFreq (const int midiNoteNumber)
{
    setFrequency (MidiMessage::getMidiNoteInHertz ((midiNoteNumber + lastPitch) + fmModulator));
    lastMidiNote = midiNoteNumber;
}

void OscData::setFmOsc (const float freq,
                        const float depth)
{
    fmDepth = depth;
    fmOsc.setFrequency (freq);
    setFrequency (MidiMessage::getMidiNoteInHertz ((lastMidiNote + lastPitch) + fmModulator));
}

void OscData::renderNextBlock (dsp::AudioBlock<float>& audioBlock)
{
    jassert (audioBlock.getNumSamples() > 0);
    process (dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process (dsp::ProcessContextReplacing<float> (audioBlock));
}

float OscData::processNextSample (float input)
{
    fmModulator = fmOsc.processSample (input) * fmDepth;
    return gain.processSample (processSample (input));
}

void OscData::setParams (const int   oscChoice,
                         const float oscGain,
                         const int   oscPitch,
                         const float fmFreq,
                         const float fmDepth)
{
    setType (oscChoice);
    setGain (oscGain);
    setOscPitch (oscPitch);
    setFmOsc (fmFreq, fmDepth);
}

void OscData::resetAll()
{
    reset();
    fmOsc.reset();
    gain.reset();
}
