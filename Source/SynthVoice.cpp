/*
  ==============================================================================

    SynthVoice.cpp
    Created: 14 May 2023 3:35:58pm
    Author:  test

  ==============================================================================
*/

#include "SynthVoice.h"

using namespace juce;

SynthVoice::SynthVoice() {}

bool SynthVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition)
{
    for (int i = 0; i < 2; i++)
    {
//        osc1[i].setFreq (MidiMessage::getMidiNoteInHertz (midiNoteNumber));
//        osc2[i].setFreq (MidiMessage::getMidiNoteInHertz (midiNoteNumber));
        osc1[i].setFreq (midiNoteNumber);
        osc2[i].setFreq (midiNoteNumber + 1);
    }
    
    adsr.noteOn();
    filterAdsr.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    filterAdsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {}
void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    reset();
    
    adsr.setSampleRate (sampleRate);
    filterAdsr.setSampleRate (sampleRate);
    
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    for (int channel = 0; channel < numChannelsToProcess; channel++)
    {
//        osc1[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
//        osc2[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
//        filter[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc1[channel].prepare (spec);
        osc1[channel].prepare (spec);
        filter[channel].prepare (spec);
        lfo[channel].prepare (spec);
        lfo[channel].initialise ([](float x) { return std::sin (x); });
    }
    
    gain.prepare (spec);
    gain.setGainLinear (0.27f);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    
    filterAdsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    filterAdsrOutput = filterAdsr.getNextSample();
    
    synthBuffer.clear();
    
    for (int channel = 0; channel < synthBuffer.getNumChannels(); ++channel)
    {
        auto* buffer = synthBuffer.getWritePointer (channel, 0);

        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            buffer[s] = osc1[channel].processNextSample (buffer[s]) + osc2[channel].processNextSample (buffer[s]);
        }
    }
    
    dsp::AudioBlock<float> audioBlock { synthBuffer };
    gain.process (dsp::ProcessContextReplacing<float> (audioBlock));
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < synthBuffer.getNumChannels(); ++channel)
    {
        auto* buffer = synthBuffer.getWritePointer (channel, 0);

        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            buffer[s] = filter[channel].processNextSample (channel, synthBuffer.getSample (channel, s));
        }
    }

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples);

        if (! adsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::reset()
{
    gain.reset();
    adsr.reset();
    filterAdsr.reset();
}

void SynthVoice::updateModParams (const int   filterType,
                                  const float filterCutoff,
                                  const float filterResonance,
                                  const float adsrDepth,
                                  const float lfoFreq,
                                  const float lfoDepth)
{
    auto cutoff = (adsrDepth * filterAdsrOutput) + filterCutoff;
    cutoff = std::clamp<float> (cutoff, 20.0f, 20000.0f);

    for (int channel = 0; channel < numChannelsToProcess; ++channel)
    {
        filter[channel].setParams (filterType, cutoff, filterResonance);
    }
}
