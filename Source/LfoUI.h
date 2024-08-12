/*
  ==============================================================================

    LfoComponent.h
    Created: 23 May 2023 1:58:37am
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BasicUI.h"

#define APVTS AudioProcessorValueTreeState

using namespace juce;

class LfoComponent
    : public BasicComponent
{
public:
    LfoComponent (APVTS& apvts,
                  String lfoFreqId,
                  String lfoDepthId);
    ~LfoComponent() override;
    
    void paint (Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = APVTS::SliderAttachment;
    
    void setSliderParams (Slider&                            slider,
                          Label&                             label,
                          std::unique_ptr<SliderAttachment>& attachment,
                          String                             paramId,
                          APVTS&                             apvts);
    
    Slider lfoFreqSlider;
    Slider lfoDepthSlider;
    
    Label lfoFreqLabel  { "Freq",  "Freq"  };
    Label lfoDepthLabel { "Depth", "Depth" };
    
    std::unique_ptr<SliderAttachment> lfoFreqAttachment;
    std::unique_ptr<SliderAttachment> lfoDepthAttachment;
    
    SliderWithLabel lfoFreq;
    SliderWithLabel lfoDepth;
    
    static constexpr int dialWidth  = 75;
    static constexpr int dialHeight = 70;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoComponent)
};
