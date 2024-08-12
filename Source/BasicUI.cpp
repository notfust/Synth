/*
  ==============================================================================

    CustomComponent.cpp
    Created: 8 May 2023 9:41:10am
    Author:  test

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BasicUI.h"

using namespace juce;

using SliderAttachment = APVTS::SliderAttachment;
using SliderStyle      = Slider::SliderStyle;


OtherLookAndFeel::OtherLookAndFeel()
{
    setColour (ComboBox::ColourIds::backgroundColourId,     Colours::lightgrey);
    setColour (ComboBox::ColourIds::outlineColourId,        Colours::black);
    setColour (ComboBox::ColourIds::textColourId,           Colours::black);
    setColour (ComboBox::ColourIds::focusedOutlineColourId, Colours::green);
    setColour (ComboBox::ColourIds::arrowColourId,          Colours::black);
    setColour (ComboBox::ColourIds::buttonColourId,         Colours::red);
}

void OtherLookAndFeel::drawRotarySlider(Graphics&   g,
                                        int         x,
                                        int         y,
                                        int         width,
                                        int         height,
                                        float       sliderPos,
                                        const float rotaryStartAngle,
                                        const float rotaryEndAngle,
                                        Slider&)
{
    auto radius  = (float) jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx      = centreX - radius;
    auto ry      = centreY - radius;
    auto rw      = radius * 2.0f;
    auto angle   = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    // fill
    g.setColour (Colours::grey);
    g.fillEllipse (rx, ry, rw, rw);

    // outline
    g.setColour (Colours::black);
    g.drawEllipse (rx, ry, rw, rw, 1.0f);
    
    Path p;
    auto pointerLength = radius * 0.8f;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));
    
    // pointer
    g.setColour (Colours::lightcyan);
    g.fillPath (p);
}

void OtherLookAndFeel::drawComboBox (Graphics& g,
                                     int       width,
                                     int       height,
                                     bool      isButtonDown,
                                     int       buttonX,
                                     int       buttonY,
                                     int       buttonW,
                                     int       buttonH,
                                     ComboBox&) {}

SliderWithLabel::SliderWithLabel (String      labelName,
                                  String      paramId,
                                  APVTS&      apvts,
                                  const int   width,
                                  const int   height,
                                  SliderStyle style)
{
    sliderWidth  = width;
    sliderHeight = height;
    
    slider.setSliderStyle  (style);
//    slider.setTextBoxStyle (Slider::NoTextBox, true, textBoxWidth, textBoxHeight);
    slider.setTextBoxStyle (Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
    slider.setColour (Slider::ColourIds::textBoxTextColourId,    Colours::black);
    slider.setColour (Slider::ColourIds::textBoxOutlineColourId, Colours::lightgrey);
    slider.setLookAndFeel (&otherLookAndFeel);
    addAndMakeVisible (slider);
    
    label.setFont (fontHeight);
    label.setColour (Label::textColourId, Colours::black);
    label.setText (labelName, dontSendNotification);
    label.setJustificationType (Justification::centredBottom);
    addAndMakeVisible (label);
    
    attachment = std::make_unique<SliderAttachment>(apvts,  paramId,  slider);
}

void SliderWithLabel::resized()
{
//    const auto dialToLabelRatio = 10;
    const auto labelHeight = 20;
    
    jassert (sliderWidth  > 0);
    jassert (sliderHeight > 0);
    
    label .setBounds (0, 0, sliderWidth, labelHeight);
    slider.setBounds (0, label.getHeight(), sliderWidth, sliderHeight - label.getHeight());
}

void SliderWithLabel::setNewValue (float newValue)
{
    slider.setValue (newValue);
    slider.setDoubleClickReturnValue (true, newValue);

}

BasicComponent::BasicComponent() {}

BasicComponent::~BasicComponent() {}

void BasicComponent::paint (Graphics& g)
{
//    g.fillAll (Colours::black);
//    auto bounds = getLocalBounds();
//    g.setColour (boundsColour);
//    g.drawRoundedRectangle (bounds.toFloat().reduced (10.0f), 5.0f, 2.0f);
    
//    g.setColour (Colours::yellow);
//    g.setFont   (fontHeight);
//    g.setFont   (g.getCurrentFont().boldened());
    
//    jassert (name.isNotEmpty());
//    g.drawText (name, 20, 15, 100, 25, Justification::left);
}

void BasicComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

