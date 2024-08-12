/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscUI.h"
#include "AdsrUI.h"
#include "BasicUI.h"
#include "FilterUI.h"
#include "LfoUI.h"

using namespace juce;


class SynthAudioProcessorEditor
    : public AudioProcessorEditor
    , public Timer
{
public:
    SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    SynthAudioProcessor& audioProcessor;
    OscComponent osc1;
    OscComponent osc2;
    FilterComponent filter;
    AdsrComponent adsr;
    LfoComponent lfo1;
    AdsrComponent filterAdsr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
