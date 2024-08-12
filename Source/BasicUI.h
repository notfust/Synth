/*
  ==============================================================================

    CustomComponent.h
    Created: 8 May 2023 9:41:10am
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define APVTS AudioProcessorValueTreeState

using namespace juce;

static constexpr float fontHeight { 15.0f };

class OtherLookAndFeel
    : public LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    
    void drawRotarySlider (Graphics&   g,
                           int         x,
                           int         y,
                           int         width,
                           int         height,
                           float       sliderPos,
                           const float rotaryStartAngle,
                           const float rotaryEndAngle,
                           Slider&) override;
    
    void drawComboBox (Graphics& g,
                       int       width,
                       int       height,
                       bool      isButtonDown,
                       int       buttonX,
                       int       buttonY,
                       int       buttonW,
                       int       buttonH,
                       ComboBox&) override;
};

class SliderWithLabel
    : public Component
{
public:
    using SliderAttachment = APVTS::SliderAttachment;
    using SliderStyle      = Slider::SliderStyle;
    
    SliderWithLabel (String      labelName,
                     String      paramId,
                     APVTS&      apvts,
                     const int   width,
                     const int   height,
                     SliderStyle style = SliderStyle::RotaryHorizontalVerticalDrag);
    
    void resized() override;
    
    void setNewValue (float newValue);
    
private:
    static constexpr int textBoxWidth  { 60 };
    static constexpr int textBoxHeight { 15 };
    int sliderWidth  { 0 };
    int sliderHeight { 0 };
    OtherLookAndFeel otherLookAndFeel;
    Slider slider;
    Label  label;
    std::unique_ptr<SliderAttachment> attachment;
};

class BasicComponent
    : public Component
{
public:
    BasicComponent();
    ~BasicComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    
    void setName         (String n) { name = n; }
    void setBoundsColour (Colour c) { boundsColour = c; }

private:
    String name { "" };
    Colour boundsColour { Colours::white };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicComponent)
};
