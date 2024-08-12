/*
  ==============================================================================

    OscComponent.cpp
    Created: 8 May 2023 9:34:57am
    Author:  test

  ==============================================================================
*/

#include "OscUI.h"

using namespace juce;

using ComboBoxAttachment = APVTS::ComboBoxAttachment;

OscComponent::OscComponent (APVTS& apvts,
                            String oscId,
                            String gainId,
                            String pitchId,
                            String fmFreqId,
                            String fmDepthId)
    : oscId   (oscId)
    , gain    ("GAIN",     gainId,    apvts, dialWidth, dialHeight)
    , pitch   ("PITCH",    pitchId,   apvts, dialWidth, dialHeight)
    , fmFreq  ("FM FREQ",  fmFreqId,  apvts, dialWidth, dialHeight)
    , fmDepth ("FM DEPTH", fmDepthId, apvts, dialWidth, dialHeight)
{
    juce::StringArray oscChoices { "Sine", "Saw", "Square", "Triangle" };
    oscSelector.addItemList (oscChoices, 1);
    oscSelector.setSelectedItemIndex (0);
    oscSelector.setLookAndFeel (&otherLookAndFeel);
    addAndMakeVisible (oscSelector);
    
    oscSelAttachment = std::make_unique<ComboBoxAttachment>(apvts, oscId, oscSelector);
    
    gain.setNewValue  (-7.5f);
    addAndMakeVisible (gain);
    addAndMakeVisible (pitch);
    addAndMakeVisible (fmFreq);
    addAndMakeVisible (fmDepth);
}

OscComponent::~OscComponent() {}

void OscComponent::paint (Graphics &g)
{
    float lineThickness = 2.0f;
    g.setFillType (Colours::lightgrey);
    g.fillRect (0, 0, 435, 140);
    
    //OSC 1 Wavetable
    g.setFillType (Colours::black);
    {
        g.drawRect (0.0f, 0.0f, 195.0f + lineThickness / 2.0f, 140.0f, lineThickness);
        
        g.drawRect (0.0f, 5.0f, 20.0f + lineThickness / 2.0f, 20.0f, lineThickness);
        g.drawRect (20.0f - lineThickness / 2.0f, 5.0f, 20.0f + lineThickness, 20.0f, lineThickness);
        g.drawRect (40.0f - lineThickness / 2.0f, 5.0f, 135.0f + lineThickness, 20.0f, lineThickness);
        g.drawRect (175.0f - lineThickness / 2.0f, 5.0f, 20.0f + lineThickness, 20.0f, lineThickness);

        g.setFillType (Colours::grey);
        g.fillRect (20.0f, 30.0f, 175.0f, 10.0f);
        g.fillRect (20.0f, 125.0f, 175.0f, 10.0f);

        g.setFillType (Colours::black);
        g.drawRect (0.0f, 30.0f, 20.0f + lineThickness / 2.0f, 105.0f, lineThickness);
        g.drawRect (20.0f - lineThickness / 2.0f, 30.0f, 175.0f + lineThickness, 105.0f, lineThickness);

        g.saveState();
        AffineTransform af;
        g.addTransform(af.rotation(-MathConstants<float>::halfPi, getWidth() * 0.5f, getHeight() * 0.5f));
        g.setFont (20.0f);
        g.drawText (this->oscId, getWidth() * 0.5f - 65.0f, getHeight() * 0.5f - 217.0f, 105.0f, 20.0f, Justification::centred, true);
        g.restoreState();
    }
    
    //OSC 1 Wavetable param
    g.setFillType (Colours::black);
    g.drawRect (195.0f - lineThickness / 2.0f, 0.0f, 240.0f + lineThickness, 140.0f, lineThickness);
}

void OscComponent::resized()
{
    const auto xStart = 195;
    const auto yStart = 0;
    const auto width  = 60;
    const auto height = 70;
    
    oscSelector.setBounds (40, 5, 135, 20);
    gain   .setBounds (xStart,                            yStart, width, height);
    pitch  .setBounds (gain  .getX() + gain  .getWidth(), yStart, width, height);
    fmFreq .setBounds (pitch .getX() + pitch .getWidth(), yStart, width, height);
    fmDepth.setBounds (fmFreq.getX() + fmFreq.getWidth(), yStart, width, height);
}
