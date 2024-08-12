/*
  ==============================================================================

    LfoComponent.cpp
    Created: 23 May 2023 1:58:37am
    Author:  test

  ==============================================================================
*/

#include "LfoUI.h"

using namespace juce;

LfoComponent::LfoComponent (APVTS& apvts,
                            String lfoFreqId,
                            String lfoDepthId)
    : lfoFreq  ("LFO FREQ",  lfoFreqId,  apvts, dialWidth, dialHeight)
    , lfoDepth ("LFO DEPTH", lfoDepthId, apvts, dialWidth, dialHeight)
{
    addAndMakeVisible (lfoFreq);
    addAndMakeVisible (lfoDepth);
}

LfoComponent::~LfoComponent() {}

void LfoComponent::paint (Graphics& g) {}

void LfoComponent::resized()
{
    const auto xStart = 0;
    const auto yStart = 0;
    const auto width  = 75;
    const auto height = 70;

    lfoFreq.setBounds (xStart, yStart, width, height);
    lfoDepth.setBounds (lfoFreq.getWidth(), yStart, width, height);
}
