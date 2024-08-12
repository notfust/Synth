/*
  ==============================================================================

    OscComponent.h
    Created: 8 May 2023 9:34:57am
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BasicUI.h"

#define APVTS AudioProcessorValueTreeState

using namespace juce;

class OscComponent
    : public BasicComponent
{
public:
    using ComboBoxAttachment = APVTS::ComboBoxAttachment;
    
    OscComponent (APVTS& apvts,
                  String oscId,
                  String gainId,
                  String pitchId,
                  String fmPitchId,
                  String fmFreqId);
    
    ~OscComponent() override;
    
    void paint (Graphics&) override;
    void resized() override;
    
private:
    String oscId;
    
    ComboBox oscSelector;
    StringArray oscChoices;
    OtherLookAndFeel otherLookAndFeel;
    std::unique_ptr<ComboBoxAttachment> oscSelAttachment;
    
    SliderWithLabel gain;
    SliderWithLabel pitch;
    SliderWithLabel fmFreq;
    SliderWithLabel fmDepth;
    
    static constexpr int dialWidth  = 60;
    static constexpr int dialHeight = 70;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
