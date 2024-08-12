/*
  ==============================================================================

    AdsrComponent.h
    Created: 8 May 2023 9:49:20am
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BasicUI.h"

#define APVTS AudioProcessorValueTreeState

using namespace juce;

class AdsrComponent
    : public BasicComponent
{
public:
    AdsrComponent (APVTS& apvts,
                   String attackId,
                   String decayId,
                   String sustainId,
                   String releaseId);
    
    ~AdsrComponent() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    OtherLookAndFeel otherLookAndFeel;
    
    SliderWithLabel attack;
    SliderWithLabel decay;
    SliderWithLabel sustain;
    SliderWithLabel release;
    
//    static constexpr int sliderWidth = 50;
//    static constexpr int sliderHeight = 260;
    
    static constexpr int dialWidth = 70;
    static constexpr int dialHeight = 70;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
