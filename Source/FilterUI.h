/*
  ==============================================================================

    FilterComponent.h
    Created: 23 May 2023 1:58:19am
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BasicUI.h"

#define APVTS AudioProcessorValueTreeState

using namespace juce;

class FilterComponent
    : public BasicComponent
{
public:
    FilterComponent (APVTS& apvts,
                     String filterTypeId,
                     String cutoffId,
                     String resonanceId);
    ~FilterComponent() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    ComboBox filterTypeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    OtherLookAndFeel otherLookAndFeel;
    
    SliderWithLabel cutoff;
    SliderWithLabel resonance;
    
    static constexpr int dialWidth = 70;
    static constexpr int dialHeight = 70;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
