/*
  ==============================================================================

    FilterComponent.cpp
    Created: 23 May 2023 1:58:19am
    Author:  test

  ==============================================================================
*/

#include "FilterUI.h"

using namespace juce;

FilterComponent::FilterComponent (APVTS& apvts,
                                  String filterTypeId,
                                  String cutoffId,
                                  String resonanceId)
    : cutoff    ("CUTOFF",    cutoffId,    apvts, dialWidth, dialHeight)
    , resonance ("RESONANCE", resonanceId, apvts, dialWidth, dialHeight)
{
    StringArray filterTypeChoices { "Low Pass", "Band Pass", "High Pass" };
    filterTypeSelector.addItemList (filterTypeChoices, 1);
    filterTypeSelector.setSelectedItemIndex (0);
    filterTypeSelector.setLookAndFeel (&otherLookAndFeel);
    addAndMakeVisible (filterTypeSelector);
    
    filterTypeAttachment = std::make_unique<APVTS::ComboBoxAttachment>(apvts, filterTypeId, filterTypeSelector);
    
    addAndMakeVisible (cutoff);
    addAndMakeVisible (resonance);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (Graphics& g)
{
    float lineThickness = 2.0f;
    g.setFillType (Colours::lightgrey);
    g.fillRect (0, 0, 345, 140);
    
    //OSC 1 Filter
    {
        g.setFillType (Colours::black);
        g.drawRect (0.0f - lineThickness / 2.0f, 0.0f, 195.0f + lineThickness, 140.0f, lineThickness);

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
        g.addTransform (af.rotation(-MathConstants<float>::halfPi, getWidth() * 0.5f, getHeight() * 0.5f));
        g.setFont (20.0f);
        g.drawText ("FILTER", getWidth() * 0.5f - 65.0f, getHeight() * 0.5f - 172.5f, 105, 20, Justification::centred, true);
        g.restoreState();
    }
    
    //OSC 1 Filter param
    g.drawRect (195.0f - lineThickness / 2.0f, 0.0f, 150.0f + lineThickness, 140.0f, lineThickness);
}

void FilterComponent::resized()
{
    const auto xStart = 195;
    const auto yStart = 0;
    const auto width  = 75;
    const auto height = 70;
    
    filterTypeSelector.setBounds (40, 5, 135, 20);
    cutoff.setBounds (xStart, yStart, width, height);
    resonance.setBounds (cutoff.getX() + cutoff.getWidth(), yStart, width, height);
}
