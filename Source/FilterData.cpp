/*
  ==============================================================================

    FilterData.cpp
    Created: 14 May 2023 3:46:25pm
    Author:  test

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterData.h"

//==============================================================================
FilterData::FilterData()
{
    setType (juce::dsp::StateVariableTPTFilterType::lowpass);
}

void FilterData::setParams (const int filterType, const float filterCutoff, const float filterResonance)
{
    selectFilterType (filterType);
    setCutoffFrequency (filterCutoff);
    setResonance (filterResonance);
}

void FilterData::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    resetAll();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    prepare (spec);
}


void FilterData::selectFilterType (const int filterType)
{
    switch (filterType)
    {
        case 0:
            setType (juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
            
        case 1:
            setType (juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
            
        case 2:
            setType (juce::dsp::StateVariableTPTFilterType::highpass);
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void FilterData::processNextBlock(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block { buffer };
    process (juce::dsp::ProcessContextReplacing<float>(block));
}

float FilterData::processNextSample (int channel, float inputValue)
{
    return processSample (channel, inputValue);
}

void FilterData::resetAll()
{
    reset();
    lfo.reset();
}
